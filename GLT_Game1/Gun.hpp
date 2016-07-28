#pragma once
#include <array>
#include <glm/vec2.hpp>
#include <GLT/MeshDrawer.hpp>
#include "SFX.hpp"

#include "Timer.hpp"

class Player;
class Bullet;

class Gun {
public:
	Player* owner;

	glm::vec2 position;
	glm::vec2 aimPosition;

	//----------

	Gun(Player* owner);
	~Gun();

	void logic();
	void draw();
	void drawUI();

	void fire();
	void gainAmmo();

	void setAimPosition(glm::vec2 position);
	glm::vec2 getAimDirection();
	float getAimDistance();

	glm::vec4 getUIColor();

protected:
	glt::MeshDrawer gunMesh;
	glt::MeshDrawer ammoMesh;
	glt::MeshDrawer crosshairMesh;
	glt::MeshDrawer crosshairLineMesh;

	std::array<Bullet*, 50> bulletList;
	size_t bulletIndex = 0;

	unsigned int ammo, maxAmmo;
	float ammoRotation = 0.f;
	float crosshairSize = 0.2f;

	Timer shootEffectTimer = Timer(0.1f);

	SFX sndShoot;
	SFX sndNoBullets;
	SFX sndPickup;

	//----------

	void loadSprites();

	glm::vec2 getTargetPosition();
	void destroyBullet(size_t index);

private:
	const float RECOIL_FORCE = 20.f;
};