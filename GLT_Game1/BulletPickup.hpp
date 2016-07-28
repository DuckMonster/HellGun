#pragma once
#include <glm/vec2.hpp>

#include "FX_Trail.hpp"
#include "Map.hpp"
#include "Actor.hpp"

class Player;

class BulletPickup : public Actor {
public:
	BulletPickup(glm::vec2 position, glm::vec2 direction, Player* player, Map* map);

	void pickup();

	void logic() override;
	void doPhysics() override;

protected:
	const float PICKUP_DISTANCE = 1.5f;
	const float MAGNET_DISTANCE = 7.f;
	const float MAGNET_FORCE = 50.f;

	Player* player;
	float angularVelocity = 5000.f;
};