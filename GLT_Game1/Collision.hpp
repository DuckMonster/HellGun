#pragma once
#include <glm\glm.hpp>

struct AABB {
public:
	static AABB fromPositionSize(glm::vec2 position, glm::vec2 size);

	AABB(glm::vec2 min, glm::vec2 max);

	glm::vec2 max() const;
	glm::vec2 min() const;
	glm::vec2 position() const;
	glm::vec2 size() const;

	bool intersects(const AABB& other) const;
	bool encapsules(const AABB& other) const;

	AABB offset(glm::vec2 offset) const;

private:
	glm::vec2 a, b;
};

bool intersects(const AABB& a, const AABB& b);
bool encapsules(const AABB& a, const AABB& b);