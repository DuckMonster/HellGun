#pragma once
#include "Entity.hpp"
#include "FX_Trail.hpp"

class Gun;

class Bullet : public Entity {
public:
	Bullet(Gun* host, glm::vec2 direction, Map* map);

	void logic() override;

protected:
	Gun* host;
	FX_Trail* trail;

private:
	const float BULLET_VELOCITY = 500.f;
	const float BULLET_OFFSET = 1.f;
};