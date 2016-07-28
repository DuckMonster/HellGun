#include "Entity.hpp"

#include "AssetBank.hpp"
#include "GameState.hpp"

using namespace glm;

Entity::Entity(Map* map) {
	this->map = map;
	velocity = glm::vec2(0.f);

	mesh.setMesh(AssetBank::loadMesh("plane"));
}

Entity::~Entity() {
}

void Entity::destroy() {
	destroyed = true;
}

void Entity::logic() {
	doPhysics();
}

void Entity::doPhysics() {
	position += velocity * GameState::deltaTime;
}

bool Entity::isOnGround() {
	return map->getCollision(getBoundingBox().offset(vec2(0.f, -0.05f)));
}

bool Entity::collidesWith(Entity& other, vec2 offset = vec2(0.f)) {
	return collidesWith(other.position + offset, other.size);
}

bool Entity::collidesWith(vec2 position, vec2 size) {
	vec2 max1 = this->position + this->size / 2.f, min1 = this->position - this->size / 2.f;
	vec2 max2 = position + size / 2.f, min2 = position - size / 2.f;

	vec2 check1 = min2 - max1;
	vec2 check2 = min1 - max2;

	return ((check1.x < 0.f && check1.y < 0.f) && (check2.x < 0.f && check2.y < 0.f));
}

AABB Entity::getBoundingBox(bool offsetToLocation) {
	AABB boundingBox = AABB::fromPositionSize(vec2(0.f), size);

	if (offsetToLocation)
		boundingBox = boundingBox.offset(position);

	return boundingBox;
}

void Entity::draw() {
	mesh.position = vec3(position, 0.f);

	if (scaleMesh)
		mesh.scale = vec3(size, 1.f);

	mesh.draw();
}