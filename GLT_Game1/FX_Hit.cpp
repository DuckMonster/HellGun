#include "FX_Hit.hpp"

#include "MathUtil.hpp"
#include "AssetBank.hpp"

using namespace glt;
using namespace glm;

FX_Hit::FX_Hit(vec2 position, vec2 direction) : FX() {
	mesh.setMesh(AssetBank::loadMesh("hit"));
	mesh.position = vec3(position, 0.f);
	mesh.rotation.z = getAngle(direction);

	size = vec2(8.f + 4.f * frand(), 8.f + 4.f * frand());
	mesh.scale = vec3(size, 1.f);
}

void FX_Hit::logic() {
	FX::logic();

	timer.update();
	if (timer.isDone())
		finish();
}

void FX_Hit::draw() {
	FX::draw();

	mesh.scale.y = size.y * (1.f - timer.value);
	mesh.scale.x = size.x + (0.6f * size.x) * timer.value;
	mesh.draw();
}