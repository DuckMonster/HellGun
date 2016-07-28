#include "Actor.hpp"

#include <glm/vec2.hpp>
#include "FX_Jump_Ground.hpp"

#include "GameState.hpp"

using namespace glm;

Actor::Actor(Map* m) : Entity(m) {
	sndLand.load("Land");
	sndLandHard.load("LandHard");
}

void Actor::doPhysics() {
	//Gravity
	velocity.y -= Physics::gravity * GameState::deltaTime;
	//Friction
	velocity.x -= velocity.x * getFriction() * GameState::deltaTime;

	//Horizontal
	if (map->getCollision(getBoundingBox().offset(velocity * vec2(GameState::deltaTime, 0.f)))) {
		velocity.x = 0.f;
	}
	if (map->getCollision(getBoundingBox().offset(velocity * vec2(0.f, GameState::deltaTime)))) {
		if (velocity.y <= -30.f) {
			sndLandHard.play();

			new FX_Jump_Ground(position + vec2(0.f, -0.5f));
			new FX_Jump_Ground(position + vec2(0.f, -0.5f));
			new FX_Jump_Ground(position + vec2(0.f, -0.5f));
		}
		else if (velocity.y < -1.f) {
			sndLand.play();
			new FX_Jump_Ground(position + vec2(0.f, -0.5f));
		}

		velocity.y = 0.f;
	}
	if (map->getCollision(getBoundingBox().offset(velocity * GameState::deltaTime))) {
		velocity = vec2(0.f);
	}

	Entity::doPhysics();
}