#include "Charger.hpp"

#include <GLT\Mesh.hpp>
#include "GameState.hpp"
#include "MathUtil.hpp"
#include "FX_Ring.hpp"

#include "Player.hpp"

Flyer::Flyer(glm::vec2 position, Player* player, Map* map) : Enemy(position, player, map) {
	chargeTimer.value = 1.f;

	float vertices[] = {
		0.5f, 0.f, 0.f,
		0.f, 0.5f, 0.f,
		0.f, -0.5f, 0.f,

		0.f, 0.5f, 0.f,
		-0.5f, 0.5f, 0.f,
		0.f, 0.f, 0.f,

		0.f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		0.f, 0.f, 0.f
	};

	mesh.setMesh(new glt::Mesh());
	mesh.getMesh()->setVertices(vertices, sizeof(vertices));

	float chargeVertices[] = {
		0.f, 0.f, 0.f,

		0.05f, 0.5f, 0.f,
		0.05f, -0.5f, 0.f,

		0.95f, 0.5f, 0.f,
		0.95f, -0.5f, 0.f,

		1.f, 0.f, 0.f,
	};

	chargeMesh.setMesh(new glt::Mesh());
	chargeMesh.getMesh()->setVertices(chargeVertices, sizeof(chargeVertices));
	chargeMesh.getMesh()->drawMode = GL_TRIANGLE_STRIP;
	chargeMesh.color = vec4(1.f, 0.f, 0.f, 0.8f);
}

void Flyer::logic() {
	if (chargeTimer.isDone()) {
		if (isCharging) 
			behaviour_Charge();
		else 
			behaviour_Idle();
	}
	else {
		chargeTimer.update();

		if (chargeTimer.isDone()) {
			isCharging = true;
			chargeTrail = new FX_Trail(this, 1.5f, 0.8f, 0.05f);
		}
	}
}

void Flyer::behaviour_Charge() {
	vec2 velo = chargeDirection * SPEED_CHARGE * GameState::deltaTime;
	RayHit<Entity> ray = map->raytrace<Entity>(AABB::fromPositionSize(vec2(0.f), size), position, position + velo, this);

	position = ray.location;
	if (ray.hit) {
		if (ray.entity == target)
			target->destroy();

		chargeTrail->release();
		chargeTrail = nullptr;

		isCharging = false;
		new FX_Ring(position, 6.f, 0.9f);

		//position += -chargeDirection * 1.f;
		velocity = -chargeDirection * 20.f;
		chargeDelay.reset();
	}
}

void Flyer::behaviour_Idle() {
	velocity -= velocity * 1.2f * GameState::deltaTime;
	velocity += normalize(target->position - position) * SPEED * GameState::deltaTime;

	if (map->getCollision<Entity>(getBoundingBox().offset(velocity * vec2(GameState::deltaTime, 0.f)), this, nullptr))
		velocity.x *= -0.4f;
	if (map->getCollision<Entity>(getBoundingBox().offset(velocity * vec2(0.f, GameState::deltaTime)), this, nullptr))
		velocity.y *= -0.4f;
	if (map->getCollision<Entity>(getBoundingBox().offset(velocity * GameState::deltaTime), this, nullptr))
		velocity *= -0.4f;
	else
		position += velocity * GameState::deltaTime;

	//Hit detection
	//Enemy::logic();

	//Delay for charging
	chargeDelay.update();
	if (chargeDelay.isDone())
		beginCharge();
}

void Flyer::beginCharge() {
	chargeDirection = normalize(target->position - position);
	chargeTimer.reset();

	RayHit<Entity> ray = map->raytrace(AABB::fromPositionSize(vec2(0.f), size), position, position + chargeDirection * 50.f);
	chargeMesh.rotation.z = getAngle(chargeDirection);
	chargeMesh.position = vec3(position, 0.f);
	chargeMesh.scale.x = length(ray.location - position);
}

void Flyer::draw() {
	if (isCharging || !chargeTimer.isDone()) {
		mesh.rotation.z = getAngle(chargeDirection);
		chargeMesh.scale.y = 1.f - chargeTimer.value;
		chargeMesh.draw();
	}
	else
		mesh.rotation.z = getAngle(target->position - position);

	mesh.position = vec3(position, 0.f);
	mesh.draw();
}