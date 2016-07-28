#include "GameState.hpp"
#include "MathUtil.hpp"
#include "AssetBank.hpp"

#include "FX_Jump_Air.hpp"
#include "Player.hpp"

using namespace glt;
using namespace glm;
using namespace std;

FX_Jump_Air::FX_Jump_Air(vec2 position, vec2 direction, Player* player) : FX() {
	this->player = player;

	float vertices[] = {
		-0.5f, 0.f, 0.f,
		0.f, -0.5f, 0.f,
		0.5f, 0.f, 0.f,
		0.f, 0.5f, 0.f,
	};

	float flashVerts[] = {
		0.f, 0.f, 0.f,
		0.2f, 0.5f, 0.f,
		1.f, 0.f, 0.f,
		0.2f, -0.5f, 0.f
	};

	direction = normalize(direction);

	this->position = position;
	this->direction = direction;

	lineMesh.setMesh(new Mesh());
	lineMesh.getMesh()->drawMode = GL_QUADS;
	lineMesh.getMesh()->setVertices(vertices, sizeof(vertices));
	lineMesh.position = vec3(position, 0.f);

	flashMesh.drawMode = GL_QUADS;
	flashMesh.setVertices(flashVerts, sizeof(flashVerts));

	float baseDirection = getAngle(direction) - 180;

	for (int i = 0; i < 12; i++) {
		float direction = baseDirection + 15.f * (frand() - 0.5f);

		float velocity = 15.f + 30.f * frand();
		float size = 0.8f + 1.5f * frand() * (1.f - velocity / 25.f);

		//smokeList.push_back(new Smoke(position, direction, velocity, size, 0.7f));
	}

	for (int i = 0; i < 3; i++) {
		float direction = baseDirection + 180 + 15.f * (frand() - 0.5f);

		float velocity = 8.f + 20.f * frand();
		float size = 0.8f + 1.5f * frand() * (1.f - velocity / 25.f);

		//smokeList.push_back(new Smoke(position, direction, velocity, size, 0.7f));
	}

	for (int i = 0; i < 5; i++) {
		float direction = baseDirection + 90.f * (frand() - 0.5f);
		float directionDif = 1.f - abs(direction - baseDirection) / 90.f;

		vec2 size = vec2(0.7f + 1.0f * frand(), 0.9f + 1.2f * frand()) * directionDif;
		vec2 offset = getVector(direction) * (0.9f + 2.5f * frand()) * directionDif;

		flashList.push_back(new Flash(&flashMesh, position, direction, size, 0.22f));
	}
}

FX_Jump_Air::~FX_Jump_Air() {
	FX::~FX();

	delete lineMesh.getMesh();

	for (int i = 0; i < smokeList.size(); i++)
		delete smokeList[i];

	for (int i = 0; i < flashList.size(); i++)
		delete flashList[i];
}

void FX_Jump_Air::logic() {
	FX::logic();

	timer.update();
	if (timer.isDone())
		finish();

	lineTimer.update();

	smokeSpawnTimer.update();
	if (smokeSpawnTimer.isDone()) {
		smokeList.push_back(new Smoke(player->position, 1.f, 0.f, 1.2f * (1.f - timer.value), 0.5f));
		smokeSpawnTimer.reset(smokeSpawnTimer.time + 1.f * timer.value);
	}

	for (int i = 0; i < smokeList.size(); i++)
		smokeList[i]->logic();

	for (int i = 0; i < flashList.size(); i++)
		flashList[i]->logic();
}

void FX_Jump_Air::draw() {
	FX::draw();

	for (int i = 0; i < smokeList.size(); i++)
		smokeList[i]->draw();

	for (int i = 0; i < flashList.size(); i++)
		flashList[i]->draw();

	float time = lineTimer.value;
	vec2 perp = vec2(-direction.y, direction.x);

	lineMesh.rotation.z = getAngle(direction);
	lineMesh.scale = vec3(1.1f - 1.1f * time, 1.f + 2.f * time, 1.f);
	//lineMesh.draw();
}

//FLASHES
FX_Jump_Air::Flash::Flash(Mesh* mesh, vec2 position, float direction, vec2 size, float time) {
	this->mesh.setMesh(mesh);
	this->mesh.position = vec3(position, 0.f);
	this->mesh.rotation.z = direction;
	this->mesh.scale = vec3(size, 1.f);

	this->size = size;
	life.reset(time);

	this->direction = getVector(direction);
}

void FX_Jump_Air::Flash::logic() {
	life.update();

	//mesh.position += vec3(direction * 20.f * (1.f - life.value) * GameState::deltaTime, 0.f);

	mesh.scale.x = size.x + 2.f * life.value;
	mesh.scale.y = size.y * (1.f - life.value);
}

void FX_Jump_Air::Flash::draw() {
	if (life.isDone())
		return;

	mesh.draw();
}

//Smoke
FX_Jump_Air::Smoke::Smoke(vec2 position, float direction, float velocity, float size, float life) {
	mesh.setMesh(AssetBank::loadMesh("smoke"));

	this->position = position;
	this->direction = direction;
	this->velocity = velocity;
	this->size = size;
	this->life.reset(life);
}

void FX_Jump_Air::Smoke::logic() {
	life.update();

	float time = 1.f - life.value;
	velocity -= velocity * GameState::deltaTime * 5.f;

	position += getVector(direction) * velocity * GameState::deltaTime;
	mesh.position = vec3(position, 0.f);
	mesh.scale = vec3(size) * time;
	mesh.color.a = 1.f - life.value;
}

void FX_Jump_Air::Smoke::draw() {
	mesh.draw();
}