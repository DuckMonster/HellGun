#include "FX_Debree.hpp"

#include "GameState.hpp"
#include "Physics.hpp"
#include "MathUtil.hpp"
#include <GLT\Mesh.hpp>

using namespace glm;
using namespace glt;

FX_Debree::FX_Debree(vec2 position, vec2 direction, float spread, int amount, vec3 color, Map* map) {
	float angle = getAngle(direction);

	for (int i = 0; i < amount; i++) {
		fragmentList.push_back(new Fragment(position, getVector(angle + spread * (frand() - 0.5f)), 0.8f, timer.time * frand(), color, map));
	}
}

FX_Debree::~FX_Debree() {
	for (int i = 0; i < fragmentList.size();i++)
		delete fragmentList[i];
}

void FX_Debree::logic() {
	FX::logic();

	for (int i = 0; i < fragmentList.size(); i++)
		fragmentList[i]->logic();

	timer.update();
	if (timer.isDone())
		finish();
}

void FX_Debree::draw() {
	FX::draw();

	for (int i = 0; i < fragmentList.size(); i++)
		fragmentList[i]->draw();
}

//FRAGMENTS
FX_Debree::Fragment::Fragment(vec2 position, vec2 direction, float size, float time, vec3 color, Map* map) : Entity(map) {
	this->map = map;

	this->position = position;
	this->velocity = direction * SPEED * 0.5f + SPEED * 0.5f * frand();
	this->color = color;

	timer.reset(time);

	float vertices[4 * 3];

	for (int i = 0; i < 4; i++) {
		vec2 v = getVector((360.f / 4.f) * i + (45.f * (frand() - 0.5f) * 2));
		v *= 0.2f + 0.8f * frand();

		vertices[i * 3] = v.x;
		vertices[i * 3 + 1] = v.y;
		vertices[i * 3 + 2] = 0.f;
	}

	mesh.setMesh(new Mesh());
	mesh.getMesh()->drawMode = GL_QUADS;
	mesh.getMesh()->setVertices(vertices, sizeof(vertices));
	mesh.scale = vec3(size);
}

FX_Debree::Fragment::~Fragment() {
	Entity::~Entity();
	delete mesh.getMesh();
}

void FX_Debree::Fragment::logic() {
	Entity::logic();

	timer.update();
}

void FX_Debree::Fragment::draw() {
	mesh.position = vec3(position, 0.f);
	mesh.color = vec4(color, 1.f - timer.value);
	mesh.draw();
}

void FX_Debree::Fragment::doPhysics() {
	//Gravity
	velocity.y -= Physics::gravity * GameState::deltaTime;
	//Friction
	//velocity -= velocity * 0.9f * GameState::deltaTime;

	Entity* collisionEntity = nullptr;

	//Horizontal
	if (map->getCollision(getBoundingBox().offset(velocity * vec2(GameState::deltaTime, 0.f)))) {
		velocity.x *= -0.2f;
	}
	if (map->getCollision(getBoundingBox().offset(velocity * vec2(0.f, GameState::deltaTime)))) {
		velocity.y *= -0.2f;
		velocity.x *= 0.2f;
	}
	if (map->getCollision(getBoundingBox().offset(velocity * GameState::deltaTime))) {
		velocity *= -0.2f;
	}

	Entity::doPhysics();
}