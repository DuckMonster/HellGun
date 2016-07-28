#include "Gun.hpp"

#include "MathUtil.hpp"
#include <vector>
#include <GLT\Mesh.hpp>
#include "AssetBank.hpp"
#include "FX_Muzzle.hpp"
#include <GLT\Time.hpp>
#include "SoundConfig.hpp"

#include "Player.hpp"
#include "Bullet.hpp"
#include "GameState.hpp"

using namespace glm;
using namespace std;
using namespace glt;

Gun::Gun(Player* owner) {
	this->owner = owner;
	this->position = owner->position;

	for (size_t i = 0; i < bulletList.size(); i++)
		bulletList[i] = nullptr;

	ammo = maxAmmo = 6;

	shootEffectTimer.value = 1.f;

	loadSprites();

	sndShoot.load("Shoot");
	sndNoBullets.load("NoBullets");

	sndPickup.load("Pickup");
	sndPickup.volume = 0.4f;
}

Gun::~Gun() {
	for (int i = 0; i < bulletList.size(); i++)
		if (bulletList[i] != nullptr)
			destroyBullet(i);
}

void Gun::loadSprites() {
	////Gun
	gunMesh.setMesh(AssetBank::loadMesh("gun"));

	////Ammo icons
	ammoMesh.setMesh(AssetBank::loadMesh("bullet"));

	////Crosshair line
	crosshairLineMesh.setMesh(new Mesh());
	crosshairLineMesh.getMesh()->drawMode = GL_LINES;

	float crosshairLineColors[] = {
		1.f, 1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f
	};

	crosshairLineMesh.getMesh()->setColors(crosshairLineColors, sizeof(crosshairLineColors));

	////Crosshair
	crosshairMesh.setMesh(new Mesh());
	crosshairMesh.getMesh()->drawMode = GL_LINE_LOOP;

	vector<float> crosshairVertices;
	int resolution = 20;
	float angle = glm::radians(360.f / resolution);

	for (int i = 0; i < resolution; i++) {
		float a = angle * i;

		crosshairVertices.push_back(cos(a));
		crosshairVertices.push_back(sin(a));
		crosshairVertices.push_back(0.f);
	}

	crosshairMesh.getMesh()->setVertices(&crosshairVertices[0], resolution * 3 * sizeof(float));
}

void Gun::fire() {
	if (ammo <= 0) {
		sndNoBullets.play();
		return;
	}

	if (bulletList[bulletIndex] != nullptr)
		destroyBullet(bulletIndex);

	//Decrease ammo and create bullet
	ammo--;

	bulletList[bulletIndex] = new Bullet(this, getAimDirection(), owner->map);
	bulletIndex = (bulletIndex + 1) % bulletList.size();

	//Create muzzle effect
	if (!GameState::getMouseInput().buttons[sf::Mouse::Right])
		new FX_Muzzle(position + getAimDirection() * 0.5f, getAimDirection());

	//Apply recoil
	owner->velocity = owner->velocity * 0.4f + normalize(aimPosition - position) * -RECOIL_FORCE;
	position -= normalize(aimPosition - position) * 1.6f;

	//Reset effect
	shootEffectTimer.reset();

	//Sounds!
	sndShoot.play();
}

void Gun::gainAmmo() {
	ammo++;
	sndPickup.play();
}

void Gun::destroyBullet(size_t index) {
	delete bulletList[index];
	bulletList[index] = nullptr;
}

void Gun::setAimPosition(glm::vec2 position) {
	this->aimPosition = position;
}

glm::vec2 Gun::getAimDirection() {
	return normalize(aimPosition - owner->position);
}

float Gun::getAimDistance() {
	return length(aimPosition - owner->position);
}

glm::vec2 Gun::getTargetPosition() {
	return owner->position + getAimDirection() * glm::min(0.5f + getAimDistance() * 0.1f, 1.6f);
}

void Gun::logic() {
	//Move towards target location, creating a "float" effect
	position += (getTargetPosition() - position) * GameState::deltaTime * 40.f;

	//Bullet logic
	for (size_t i = 0; i < bulletList.size(); i++)
		if (bulletList[i] != nullptr) {
			bulletList[i]->logic();

			if (bulletList[i]->isDestroyed())
				destroyBullet(i);
		}

	//Create the bullet rotation effect
	ammoRotation += ((-360.f / maxAmmo) * (maxAmmo - ammo) - ammoRotation) * GameState::deltaTime * 10.f;

	//Shooting effect (with expanding crosshair)
	shootEffectTimer.update();
	crosshairSize = 0.2f + 0.5f * (1.f - shootEffectTimer.value);

	//Update aiming line!
	vec2 crossHairHalfway = position + (aimPosition - position) * 0.5f;
	vec2 crossHairOffset = normalize(position - aimPosition) * crosshairSize;

	float lineVerts[] = {
		crossHairHalfway.x, crossHairHalfway.y, 0.f,
		(aimPosition + crossHairOffset).x, (aimPosition + crossHairOffset).y, 0.f
	};

	crosshairLineMesh.getMesh()->setVertices(lineVerts, sizeof(lineVerts));
	//
}

void Gun::draw() {
	//Draw gun
	gunMesh.position = vec3(position, 0.f);
	gunMesh.rotation.z = getAngle(aimPosition - position);

	if (gunMesh.rotation.z <= -90 || gunMesh.rotation.z >= 90)
		gunMesh.scale.y = -1.f;
	else
		gunMesh.scale.y = 1.f;

	gunMesh.draw();

	//Draw bullets
	for (size_t i = 0; i < bulletList.size(); i++)
		if (bulletList[i] != nullptr) bulletList[i]->draw();
}

vec4 Gun::getUIColor() {
	if (ammo <= 2)
		return vec4(1.f) + (vec4(1.f, 0.f, 0.f, 1.f) - vec4(1.f)) * ((float)sin(Time::total() * 20.f) * 0.5f + 0.5f);
	else
		return vec4(1.f);
}

void Gun::drawUI() {
	vec4 uiColor = getUIColor();
	ammoMesh.color = uiColor;
	crosshairLineMesh.color = uiColor;
	crosshairMesh.color = uiColor;

	//Draw ammo
	for (size_t i = 0; i < ammo; i++) {
		float angle = -360.f / maxAmmo;
		float rotation = ammoRotation + angle * (i + 1) + 90.f;

		ammoMesh.position = vec3(aimPosition + getVector(rotation) * 1.f, 0.f);
		ammoMesh.rotation.z = rotation;
		ammoMesh.scale = vec3(0.6f);
		ammoMesh.draw();
	}

	//Shoot effect - make the bullet "pop out" when shot
	if (!shootEffectTimer.isDone()) {
		ammoMesh.position = vec3(aimPosition + getVector(90.f) + vec2(0.f, 2.f * shootEffectTimer.value), 0.f);
		ammoMesh.scale = vec3(1.f + 0.5f * (1.f - shootEffectTimer.value));
		ammoMesh.rotation.z = 90.f;
		ammoMesh.color.a = 1.f - shootEffectTimer.value;
		ammoMesh.draw();
	}

	//Crosshair line and circle
	crosshairLineMesh.draw();

	crosshairMesh.position = vec3(aimPosition, 0.f);
	crosshairMesh.scale = vec3(crosshairSize);
	crosshairMesh.draw();
}