#include "KillCounter.hpp"

#include <string>
#include "GameState.hpp"
#include "AssetBank.hpp"
#include "MathUtil.hpp"
#include "Player.hpp"
#include "Gun.hpp"

#include "Map.hpp"

using namespace glm;

KillCounter::KillCounter(Map* map) {
	this->map = map;

	mesh.setFont(AssetBank::loadFont("dungeon"));
	mesh.setText("0");

	drawer.setMesh(mesh.getMesh());
	drawer.texture = mesh.getFont()->getTexture();
	drawer.scale = vec3(10.f);
	drawer.position.z = -10.f;
}

void KillCounter::increment() {
	kills++;
	mesh.setText(std::to_string(kills));

	animTimer.reset();
}

void KillCounter::logic() {
	animTimer.update();
}

glm::vec4 KillCounter::getColor() {
	return idleColor + (killColor - idleColor) * (1.f - linToExp(animTimer.value));
}

void KillCounter::draw() {
	if (map->getGameOver())
		return;

	drawer.color = getColor();

	vec2 pos = map->getPlayer()->position;
	pos += map->getPlayer()->getGun()->getAimDirection() * -3.f;

	drawer.position = vec3(pos, 0.f);

	drawer.scale = vec3(3.f) + vec3(2.f, 5.f, 0.f) * (1.f - linToExp(animTimer.value));
	drawer.draw();
}