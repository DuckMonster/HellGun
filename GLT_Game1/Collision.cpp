#include "Collision.hpp"

AABB AABB::fromPositionSize(glm::vec2 position, glm::vec2 size) {
	return AABB(position - size / 2.f, position + size / 2.f);
}

AABB::AABB(glm::vec2 a, glm::vec2 b) {
	this->a = a;
	this->b = b;
}

glm::vec2 AABB::max() const {
	return glm::max(a, b);
}

glm::vec2 AABB::min() const {
	return glm::min(a, b);
}

glm::vec2 AABB::position() const {
	return min();
}

glm::vec2 AABB::size() const {
	return max() - min();
}

bool AABB::intersects(const AABB& other) const {
	return
		max().x >= other.min().x &&
		max().y >= other.min().y &&
		min().x < other.max().x &&
		min().y < other.max().y;
}

bool AABB::encapsules(const AABB& other) const {
	return
		min().x <= other.min().x &&
		min().y <= other.min().y &&
		max().x >= other.max().x &&
		max().y >= other.max().y;
}

AABB AABB::offset(glm::vec2 offset) const {
	AABB copy(a, b);
	copy.a += offset;
	copy.b += offset;
	return copy;
}

bool intersects(const AABB& a, const AABB& b) {
	return a.intersects(b);
}