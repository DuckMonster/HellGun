#include "FX_Kill.hpp"

#include "ColorConfig.hpp"
#include "AssetBank.hpp"
#include "MathUtil.hpp"
#include "GameState.hpp"

using namespace glm;
using namespace std;

FX_Kill::FX_Kill(vec2 position, vec2 direction) {
	for (int i = 0; i < 30; i++) {
		float len = frand() * 1.f;
		vec2 pos = getVector(frand() * 360.f) * len;
		float size = 4.f * (len / 2.f);

		smokeCloud.push_back(Smoke(position + pos, size, 16.f * frand()));
	}

	for (int i = 0; i < 5; i++) {
		debreeList.push_back(Debree(position, getAngle(direction) + 80.f * (frand() - 0.5f)));
	}
}

void FX_Kill::logic() {
	timer.update();
	if (timer.isDone())
		finish();

	for (int i = 0; i < smokeCloud.size(); i++)
		smokeCloud[i].logic();

	for (int i = 0; i < debreeList.size(); i++)
		debreeList[i].logic();
}

void FX_Kill::draw() {
	for (int i = 0; i < smokeCloud.size(); i++)
		smokeCloud[i].draw();

	for (int i = 0; i < debreeList.size(); i++)
		debreeList[i].draw();
}

//Smoke

FX_Kill::Smoke::Smoke(vec2 position, float size, float velocity) {
	mesh.setMesh(AssetBank::loadMesh("smoke"));
	mesh.position = vec3(position, 0.f);
	mesh.color = ColorConfig::getEnemy() * vec4(vec3(0.8f + 0.2f * frand()), 1.f);

	this->size = size;
	this->velocity = getVector(frand() * 360.f) * velocity;
}

void FX_Kill::Smoke::logic() {
	timer.update();

	velocity -= velocity * 3.f * GameState::deltaTime;
	mesh.position += vec3(velocity, 0.f) * GameState::deltaTime;
}

void FX_Kill::Smoke::draw() {
	float beginTime = clamp(timer.value / 0.05f, 0.f, 1.f);

	mesh.scale = vec3(size) * (1.f - timer.value) * beginTime;
	mesh.draw();
}

//Debree

FX_Kill::Debree::Debree(vec2 position, float direction) {
	this->position = position;
	velocity = getVector(direction) * (40.f + 50.f * frand());
	smokeTimer.value = 1.f;
}

void FX_Kill::Debree::logic() {
	//velocity.y -= 40.f * GameState::deltaTime;
	velocity -= velocity * 4.5f * GameState::deltaTime;
	position += velocity * GameState::deltaTime;

	flyTimer.update();

	if (!flyTimer.isDone()) {
		smokeTimer.update();

		if (smokeTimer.isDone()) {
			smokeList.push_back(Smoke(position + vec2((frand() - 0.5f) * 0.4f, (frand() - 0.5f) * 0.4f), 1.f * (1.f - flyTimer.value), 0.f));
			smokeTimer.reset();
		}
	}

	for (int i = 0; i < smokeList.size(); i++)
		smokeList[i].logic();
}

void FX_Kill::Debree::draw() {
	for (int i = 0; i < smokeList.size(); i++)
		smokeList[i].draw();
}