#pragma once
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>

#include "Input.hpp"
#include "Actor.hpp"
#include "CameraController.hpp"
#include "FX_Trail.hpp"
#include "SFX.hpp"

const unsigned int BUTTON_JUMP = sf::Keyboard::Space;
const unsigned int BUTTON_RIGHT = sf::Keyboard::D;
const unsigned int BUTTON_LEFT = sf::Keyboard::A;
const unsigned int BUTTON_DOWN = sf::Keyboard::S;
const unsigned int BUTTON_DASH = sf::Keyboard::LShift;
const unsigned int BUTTON_DASH_MOUSE = sf::Mouse::Right;
const unsigned int BUTTON_FIRE = sf::Mouse::Left;

class Gun;
 
class Player : public Actor {
public:
	Player(Map* map);
	~Player();
	void logic() override;
	void draw() override;
	void drawUI();

	void destroy() override;

	Gun* getGun();

protected:
	unsigned int ID = 0;

	//-----------

	void doInput();
	float getAcceleration() { return isOnGround() ? ACCELERATION : ACCELERATION * Physics::airFraction; }
	int getWallGrab();
	void dash(int dir);
	void doDash();

private:
	//PHYSICS CONSTANTS
	const float JUMP_STRENGTH = 17.f;
	const float JUMP_HOLD = 15.f;
	const float JUMP_HOLD_CAP = 7.f;
	const float ACCELERATION = 350.f;
	const float MAX_SPEED = 30.f;
	const float WALL_SLIDE_FRICTION = 5.f;
	const glm::vec2 WALL_JUMP_FORCE = glm::vec2(-25.f, 15.f);

	const float FAST_FALL_SPEED = 40.f;

	const float DASH_INPUT_TIME = 0.17f;
	const float DASH_DISTANCE = 4.5f;
	const float DASH_SPEED = 60.f;

	const bool DASH_DOUBLE_TAP = false;
	//

	bool alive = true;

	int jumpStorage = 0;
	float wallStick = 0.f;
	KeyboardState k_prevInput;
	MouseState m_prevInput;

	Timer dashInput = Timer(DASH_INPUT_TIME);
	int dashDir = 0;
	bool isDashing = false;
	vec2 dashTarget;

	FX_Trail* dashTrail;

	Gun* gun;

	SFX sndJump;
	SFX sndDash;
	SFX sndDie;
};