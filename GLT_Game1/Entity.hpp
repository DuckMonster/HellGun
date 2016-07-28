#pragma once
#include <GLT\MeshDrawer.hpp>
#include <glm\vec2.hpp>
#include "Input.hpp"
#include "Map.hpp"
#include "Physics.hpp"

class Entity {
public:
	Map* map;

	glm::vec2 size = glm::vec2(1.f);
	glt::MeshDrawer mesh;

	glm::vec2 position = glm::vec2(0.f);
	glm::vec2 velocity = glm::vec2(0.f);

	bool collisionEnabled = true;

	//------------

	Entity(Map*);
	virtual ~Entity();

	virtual void logic();
	virtual void draw();

	bool isOnGround();
	float getFriction() { return Physics::friction * (isOnGround() ? 1.f : Physics::airFraction); }
	virtual bool collidesWith(Entity& other, glm::vec2);
	virtual bool collidesWith(glm::vec2, glm::vec2);

	bool isDestroyed() { return destroyed; }
	virtual void destroy();

	AABB getBoundingBox(bool offsetToLocation = true);

protected:
	bool destroyed = false;
	float bounciness = 0.f;
	bool scaleMesh = false;

	//---------

	virtual void doPhysics();

private:
};