#include "BulletPickup.hpp"

#include "MathUtil.hpp"
#include "GameState.hpp"
#include "FX_Ring.hpp"
#include "AssetBank.hpp"

#include "Player.hpp"
#include "Gun.hpp"

using namespace glm;

BulletPickup::BulletPickup(vec2 position, vec2 direction, Player* player, Map* map) : Actor(map) {
	this->position = position + direction * 1.f;
	direction = normalize(direction + vec2(0, 0.8f));

	this->player = player;
	velocity = direction * 20.f;

	collisionEnabled = false;
	size = vec2(0.8f, 0.4f);

	angularVelocity = 1000.f + 3000.f * frand();

	//Mesh
	mesh.setMesh(AssetBank::loadMesh("bullet"));
	mesh.scale = vec3(0.6f);
	scaleMesh = false;

	//Clamp position into arena
	this->position = clamp(this->position, -map->getSize() / 2.f + size, map->getSize() / 2.f - size);
}

void BulletPickup::pickup() {
	player->getGun()->gainAmmo();
	new FX_Ring(position, 2.f, 0.2f);

	destroy();
}

void BulletPickup::logic() {
	mesh.rotation.z += angularVelocity * GameState::deltaTime;
	angularVelocity -= angularVelocity * 0.6f * GameState::deltaTime;

	Actor::logic();

	float distanceToPlayer = length(player->position - position);
	if (distanceToPlayer <= PICKUP_DISTANCE)
		pickup();
	else if (distanceToPlayer <= MAGNET_DISTANCE) {
		vec2 magnetForce =
			normalize(player->position - position) *
			(1.f - distanceToPlayer / MAGNET_DISTANCE) *
			MAGNET_FORCE;

		velocity += magnetForce * GameState::deltaTime;
	}
}

void BulletPickup::doPhysics() {
	//Gravity
	velocity.y -= Physics::gravity * 0.8f * GameState::deltaTime;
	//Friction
	velocity -= velocity * 0.9f * GameState::deltaTime;

	Entity* collisionEntity = nullptr;

	//Horizontal
	if (map->getCollision(getBoundingBox().offset(velocity * vec2(GameState::deltaTime, 0.f)))) {
		velocity.x *= -0.8f;
	}
	if (map->getCollision(getBoundingBox().offset(velocity * vec2(0.f, GameState::deltaTime)))) {
		velocity.y *= -0.8f;
		velocity.x *= 0.6f;

		angularVelocity *= 0.5f;
	}
	if (map->getCollision(getBoundingBox().offset(velocity * GameState::deltaTime))) {
		velocity *= -0.8f;
	}

	if (collisionEntity == player)
		pickup();

	Entity::doPhysics();
}