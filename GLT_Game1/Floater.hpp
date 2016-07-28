#pragma once
#include <vector>
#include "Enemy.hpp"
#include "FX_Chain.hpp"

class Floater : public Enemy {
public:
	Floater(glm::vec2 position, Player* target, Map* map);
	~Floater() override;

	void logic() override;
	void draw() override;

	void updateDirection();

private:
	const float FLOAT_SPEED = 10.f;
	const float TURN_SPEED = 50.f;
	const float BITE_ANGLE = 85.f;

	float speedFactor = 1.f;
	float driftValue = 0.f;

	float biteValue = 0.f;

	float direction = 0.f;
	FX_Chain* chain[3];

	Timer chainUpdateTimer = Timer(0.01f);

	glt::MeshDrawer upperMesh;
	glt::MeshDrawer lowerMesh;
	glt::MeshDrawer bodyMesh;

	//Animation timers
	Timer animBegin = Timer(0.4f);

	//-------------

	float getDrift();
	float getTargetBite();
};