#include "Player.hpp"

#include <glm\vec4.hpp>
#include "Game.hpp"
#include "GameState.hpp"
#include "Enemy.hpp"
#include "FX_Ring.hpp"
#include "FX_Trail.hpp"
#include "FX_Debree.hpp"
#include "FX_Hit.hpp"
#include "FX_Jump_Ground.hpp"
#include "FX_Jump_Air.hpp"
#include "ColorConfig.hpp"
#include "SoundConfig.hpp"
#include "AssetBank.hpp"

#include "Gun.hpp"

using namespace glm;

Player::Player(Map* map) : Actor(map) {
	mesh.color = ColorConfig::getPlayer();

	gun = new Gun(this);

	k_prevInput = GameState::getKeyInput();
	m_prevInput = GameState::getMouseInput();

	sndJump.load("Jump");

	sndDash.load("Dash");
	sndDash.volume = 0.4f;

	sndDie.load("Die");
}

Player::~Player() {
	delete gun;
}

void Player::destroy() {
	if (!alive)
		return;

	collisionEnabled = false;

	map->playerDie();
	alive = false;

	//new FX_Ring(position, 8.f, 1.5f);
	new FX_Debree(position, vec2(1.f, 0.f), 360.f, 10, vec3(1.f, 1.f, 1.f), map);
	sndDie.play();
}

int Player::getWallGrab() {
	//Dont wallgrab if you're on the ground, silly!
	if (isOnGround())
		return 0;

	int dir = 0;

	if (map->getCollision(getBoundingBox().offset(vec2(0.1f, 0.f))))
		dir++;
	if (map->getCollision(getBoundingBox().offset(vec2(-0.1f, 0.f))))
		dir--;

	return dir;
}

Gun* Player::getGun() { return gun; }

void Player::logic() {
	if (!alive) return;

	//Gun stuff
	gun->setAimPosition(GameState::currentInput.mouse.world);
	gun->logic();

	if (!isDashing) {
		int prevWallGrab = getWallGrab();

		if (alive)
			doInput();

		Actor::logic();

		//If touching the wall after moving, stick to wall
		//This is to prevent not sticking if touching the wall without pressing any input
		if (prevWallGrab == 0 && prevWallGrab != getWallGrab()) {
			wallStick = 1.f;
		}

		//Glide on wall (Unless wallstick reaches zero)
		if (wallStick > 0.f && getWallGrab() != 0) {
			velocity.y -= velocity.y * WALL_SLIDE_FRICTION * GameState::deltaTime;

			//Reduce wallstick automatically if the player doesn't input anything
			wallStick -= 1.5f * GameState::deltaTime;

			//If gliding on wall, reset jump storage
			jumpStorage = 1;
		}

		//Reset jump storage if on ground
		if (isOnGround())
			jumpStorage = 1;
	}
	else {
		doDash();
	}
}

void Player::doInput() {
	KeyboardState k_input = GameState::getKeyInput();
	MouseState m_input = GameState::getMouseInput();
	int currentWallGrab = getWallGrab();

	//DO INPUT HERE
	//Actions
	if (k_input.keys[BUTTON_JUMP] && !k_prevInput.keys[BUTTON_JUMP]) { //JUMP
		if (isOnGround()) {
			//Ground jumping
			velocity.y = JUMP_STRENGTH;
			sndJump.play();

			new FX_Jump_Ground(position + vec2(0.f, -0.5f));
		}
		else {
			//Walljumping
			if (currentWallGrab != 0) {
				velocity = WALL_JUMP_FORCE * vec2(currentWallGrab, 1.f);
				sndJump.play();

				wallStick = 0.f;
			}

			//Double jumping
			else if (jumpStorage > 0) {
				velocity.y = JUMP_STRENGTH;
				jumpStorage--;
				sndJump.play();

				new FX_Jump_Air(position, velocity, this);
			}
		}
	}
	//Holding jump
	if (k_input.keys[BUTTON_JUMP] && velocity.y >= JUMP_HOLD_CAP) {
		velocity.y += JUMP_HOLD * GameState::deltaTime;
	}

	//Movement
	float movementInput = 0.f;
	if (k_input.keys[BUTTON_RIGHT]) movementInput += 1.f;
	if (k_input.keys[BUTTON_LEFT]) movementInput -= 1.f;

	if (abs(movementInput) >= 0.2f) {
		float deltav = getAcceleration() * movementInput * GameState::deltaTime;

		//If wallsticking, set deltav to 0
		if (wallStick > 0.f) {
			deltav = 0.f;
		}
		// COOL CODE BROthisfuc
		////Do the movement stuff
		//Right movement cap
		if (velocity.x + deltav >= MAX_SPEED)
			velocity.x += max(MAX_SPEED - velocity.x, 0.f);

		//Left movement cap
		else if (velocity.x + deltav <= -MAX_SPEED)
			velocity.x += min(-MAX_SPEED - velocity.x, 0.f);

		//Otherwise just go fast
		else
			velocity.x += deltav;

		////Wall sticking
		//Remove wall sticking if not actually touching a wall
		if (currentWallGrab == 0)
			wallStick = 0.f;
		//If pressing into a wall, reset wallsticking
		else if (sign(movementInput) == sign(currentWallGrab))
			wallStick = 1.f;
		//Otherwise (Pressing away) remove wallstick faster
		else
			wallStick -= 1.5f * GameState::deltaTime;
	}

	//Fall-fast
	if (k_input.keys[BUTTON_DOWN] && !k_prevInput.keys[BUTTON_DOWN]) {
		velocity = vec2(0.f, -FAST_FALL_SPEED);
		wallStick = 0.f;

		new FX_Jump_Air(position, velocity, this);
		sndDash.play();
	}

	//Dashing
	if (DASH_DOUBLE_TAP) {
		dashInput.update();

		if (k_input.keys[BUTTON_RIGHT] && !k_prevInput.keys[BUTTON_RIGHT]) {
			if (!dashInput.isDone() && dashDir == 1) {
				dash(dashDir);
			}
			else {
				dashDir = 1;
				dashInput.reset();
			}
		}
		if (k_input.keys[BUTTON_LEFT] && !k_prevInput.keys[BUTTON_LEFT]) {
			if (!dashInput.isDone() && dashDir == -1) {
				dash(dashDir);
			}
			else {
				dashDir = -1;
				dashInput.reset();
			}
		}
	}

	if (k_input.keys[BUTTON_DASH] && !k_prevInput.keys[BUTTON_DASH] ||
		m_input.buttons[BUTTON_DASH_MOUSE] && !m_prevInput.buttons[BUTTON_DASH_MOUSE]) {
		if (abs(movementInput) >= 0.2f)
			dash((int)movementInput);
	}

	//Shooting
	if (m_input.buttons[BUTTON_FIRE] && !m_prevInput.buttons[BUTTON_FIRE])
		gun->fire();

	//----

	k_prevInput = k_input;
	m_prevInput = m_input;
}

void Player::dash(int dir) {
	dashDir = glm::clamp(dir, -1, 1);
	dashTarget = position + vec2(dashDir * DASH_DISTANCE, 0.f);
	isDashing = true;

	dashInput.value = 1.f;
	dashTrail = new FX_Trail(this, 0.6f, 0.7f, 0.1f);

	sndDash.play();
}

void Player::doDash() {
	vec2 delta = normalize(dashTarget - position) * DASH_SPEED * GameState::deltaTime;

	float d = length(delta);
	float v = length(dashTarget - position);

	if (length(delta) >= length(dashTarget - position))
		delta = dashTarget - position;

	RayHit<Entity> ray = map->raytrace<Entity>(getBoundingBox(false), position, position + delta, this);
	position = ray.location;

	if (ray.hit) {
		Enemy* e = dynamic_cast<Enemy*>(ray.entity);

		if (e != nullptr)
			destroy();

		isDashing = false;
		velocity = vec2(0.f);
		dashTrail->release();
	}

	if (length(position - dashTarget) <= 0.1f) {
		position = dashTarget;
		velocity = normalize(delta) * 50.f;

		isDashing = false;
		dashTrail->release();
	}
}

void Player::draw() {
	if (!alive) return;

	Actor::draw();
	gun->draw();
}

void Player::drawUI() {
	if (!alive) return;

	gun->drawUI();
}