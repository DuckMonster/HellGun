#include "FX_Muzzle.hpp"

#include "AssetBank.hpp"
#include "MathUtil.hpp"
#include "GameState.hpp"

using namespace glt;
using namespace glm;

FX_Muzzle::FX_Muzzle(vec2 position, vec2 direction) : FX() {
	//Create flash meshes
	float vertices_small[] = {
		0.f, 0.f, 0.f,
		0.2f, -0.5f, 0.f,
		1.f, 0.f, 0.f,
		0.2f, 0.5f, 0.f
	};

	float vertices_big[] = {
		-1.f, 0.f, 0.f,
		0.f, -0.5f, 0.f,
		1.f, 0.f, 0.f,
		0.f, 0.5f, 0.f
	};

	smallFlash.drawMode = GL_QUADS;
	smallFlash.setVertices(vertices_small, sizeof(vertices_small));

	bigFlash.drawMode = GL_QUADS;
	bigFlash.setVertices(vertices_big, sizeof(vertices_big));

	//Fireangle in degrees
	float fireAngle = getAngle(direction);

	//Create 2 big flashes
	//One perpendicular right at the muzzle
	flashList.push_back(new Flash(&bigFlash, position, fireAngle - 90, vec2(0.6f + 1.2f * frand(), 0.7f + 1.5f * frand()), 0.1f));
	//And one parallel, the main flash
	flashList.push_back(new Flash(&smallFlash, position, fireAngle, vec2(6.f, 2.f), 0.1f));

	//3 smaller flashes
	for (int i = 0; i < 3; i++) {
		vec2 size = vec2(2.f + frand() * 2.f, 1.f + frand() * 2.f);
		float dir = fireAngle + 45.f * (frand() - 0.5f);

		size *= 1.f - abs(dir - fireAngle) / 20.f;

		flashList.push_back(new Flash(&smallFlash, position, dir, size, 0.1f));
	}

	//Create smoke
	for (int i = 0; i < 4; i++) {
		float dir = fireAngle + (frand() - 0.5f) * 45.f;
		float dirDif = 1.f - (abs(dir - fireAngle) / 15.f);

		float velocity = 2.f + 30.f * frand() * dirDif;
		vec2 offset = getVector(dir) * (0.5f + 6.f * frand() * dirDif);

		float size = 0.8f + 1.5f * frand() * (velocity / 35.f) * (1.f - length(offset) / 6.f);
		float life = 0.3f + 0.5f * frand();

		sparkList.push_back(new Spark(AssetBank::loadMesh("smoke"), position + offset, size, dir, velocity, life));
	}
}

FX_Muzzle::~FX_Muzzle() {
	FX::~FX();

	for (int i = 0; i < flashList.size(); i++)
		delete flashList[i];

	for (int i = 0; i < sparkList.size(); i++)
		delete sparkList[i];
}

void FX_Muzzle::logic() {
	FX::logic();

	for (int i = 0; i < flashList.size(); i++)
		flashList[i]->logic();
	for (int i = 0; i < sparkList.size(); i++)
		sparkList[i]->logic();

	timer.update();
	if (timer.isDone())
		finish();
}

void FX_Muzzle::draw() {
	FX::draw();

	for (int i = 0; i < flashList.size(); i++)
		flashList[i]->draw();
	for (int i = 0; i < sparkList.size(); i++)
		sparkList[i]->draw();
}

//FLASHES
FX_Muzzle::Flash::Flash(Mesh* mesh, vec2 position, float direction, vec2 size, float time) {
	this->mesh.setMesh(mesh);
	this->mesh.position = vec3(position, 0.f);
	this->mesh.rotation.z = direction;
	this->mesh.scale = vec3(size, 1.f);

	this->size = size;
	life.reset(time);
}

void FX_Muzzle::Flash::logic() {
	life.update();

	mesh.scale.x = size.x + 2.f * life.value;
	mesh.scale.y = size.y * (1.f - life.value);
}

void FX_Muzzle::Flash::draw() {
	if (life.isDone())
		return;

	mesh.draw();
}

//SPARK
FX_Muzzle::Spark::Spark(Mesh* mesh, vec2 position, float size, float direction, float velocity, float life) {
	this->mesh.setMesh(mesh);
	this->position = position;
	this->size = vec2(size + size*0.4f * (frand() - 0.5f), size + size*0.4f * (frand() - 0.5f));
	this->direction = direction;
	this->velocity = velocity;
	this->life.reset(life);

	angularVelocity = (frand() - 0.5f) * 400.f;
}

void FX_Muzzle::Spark::logic() {
	life.update();

	float perc = 1.f - life.value;

	direction += angularVelocity * GameState::deltaTime * perc;
	position += getVector(direction) * velocity * GameState::deltaTime * perc;

	mesh.position = vec3(position, 0.f);
	mesh.rotation.z = direction - 180;
	mesh.scale = vec3(size, 1.f) * perc;
	mesh.color.a = perc;
}

void FX_Muzzle::Spark::draw() {
	if (life.isDone())
		return;

	mesh.draw();
}