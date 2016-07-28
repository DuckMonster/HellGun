#include "GameState.hpp"
#include "MathUtil.hpp"
#include "AssetBank.hpp"

#include "FX_Jump_Ground.hpp"

using namespace glt;
using namespace glm;

FX_Jump_Ground::FX_Jump_Ground(vec2 position) : FX() {
	for (int d = 0; d < 2; d++) {
		for (int i = 0; i < 2; i++) {
			float direction = 0;
			if (d == 0)
				direction = 0 + 10.f * frand();
			else
				direction = 180 - 10.f * frand();

			float velocity = 5.f + 20.f * frand();
			float size = 0.8f + 1.2f * frand() * (1.f - velocity / 25.f);

			smokeList.push_back(new Smoke(position, direction, velocity, size, 0.7f));
		}
	}
}

FX_Jump_Ground::~FX_Jump_Ground() {
	FX::~FX();

	for (int i = 0; i < smokeList.size(); i++)
		delete smokeList[i];
}

void FX_Jump_Ground::logic() {
	FX::logic();

	timer.update();
	if (timer.isDone())
		finish();

	for (int i = 0; i < smokeList.size(); i++)
		smokeList[i]->logic();
}

void FX_Jump_Ground::draw() {
	FX::draw();

	for (int i = 0; i < smokeList.size(); i++)
		smokeList[i]->draw();
}

//Smoke
FX_Jump_Ground::Smoke::Smoke(vec2 position, float direction, float velocity, float size, float life) {
	mesh.setMesh(AssetBank::loadMesh("smoke"));

	this->position = position;
	this->direction = direction;
	this->velocity = velocity;
	this->size = size;
	this->life.reset(life);
}

void FX_Jump_Ground::Smoke::logic() {
	life.update();

	float time = 1.f - life.value;
	velocity -= velocity * GameState::deltaTime * 5.f;

	position += getVector(direction) * velocity * GameState::deltaTime;
	mesh.position = vec3(position, 0.f);
	mesh.scale = vec3(size) * time;
	mesh.color.a = 1.f - life.value;
}

void FX_Jump_Ground::Smoke::draw() {
	mesh.draw();
}