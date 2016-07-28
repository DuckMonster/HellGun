#pragma once
#include "Enemy.hpp"

#include "FX_Trail.hpp"

class Flyer : public Enemy {
public:
	Flyer(glm::vec2 position, Player* player, Map* map);

	void logic() override;
	void draw() override;

private:
	const float SPEED = 5.f;
	const float SPEED_CHARGE = 500.f;

	Timer chargeTimer = Timer(1.f);
	Timer chargeDelay = Timer(4.f);
	bool isCharging = false;
	vec2 chargeDirection;
	FX_Trail* chargeTrail;

	glt::MeshDrawer chargeMesh;

	//---------

	void beginCharge();

	void behaviour_Charge();
	void behaviour_Idle();
};