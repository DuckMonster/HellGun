#include "Floater_Spawner.hpp"

#include <GLT\Time.hpp>
#include "GameState.hpp"
#include "FX_Ring.hpp"
#include "ColorConfig.hpp"
#include "MathUtil.hpp"

#include "AssetBank.hpp"

#include "Floater.hpp"
#include "Charger.hpp"
#include "Player.hpp"
#include "MathUtil.hpp"

SFX Spawner::sndEnter("SpawnEnter");
SFX Spawner::sndSpawn("Spawn");

Spawner::Spawner(glm::vec2 position, int spawnSize, float beginDelay, Map* map) : Entity(map) {
	this->position = position;
	this->spawnSize = spawnSize;
	this->beginDelay = Timer(beginDelay);

	pentagramMesh.setMesh(AssetBank::loadMesh("spawner"));
	pentagramMesh.color = ColorConfig::getEnemy();

	ringMesh.setMesh(AssetBank::loadMesh("spawner", 1));
	ringMesh.color = ColorConfig::getEnemy();

	mesh.position = pentagramMesh.position = ringMesh.position = vec3(position, 0.f);
	collisionEnabled = false;

	new FX_Ring(position, 5.f, 0.5f);

	sndEnter.volume = 0.4f;
	sndEnter.play();
}

void Spawner::destroy() {
	map->enemyKilled(false);
	Entity::destroy();
}

float Spawner::getSize() {
	return scale * beginDelay.value * (1.f - endDelay.value);
}

void Spawner::logic() {
	rotation += GameState::deltaTime * 80.f;
	pentagramMesh.rotation.z += GameState::deltaTime * 100.f * beginDelay.value;
	pentagramMesh.rotation.x += GameState::deltaTime * 120.f * beginDelay.value;

	scale += (1.f - scale) * GameState::deltaTime * 1.5f;

	pentagramScale = 1.f + sin(glt::Time::total()) * 0.5f;

	//Beginning spawn delay
	beginDelay.update();

	if (beginDelay.isDone()) {
		spawnDelay.update();

		//If finished
		//Run the ending delay (mesh disappearing)
		if (finished()) {
			endDelay.update();
			if (endDelay.isDone())
				destroy();
		}

		//Else spawn monsters!
		else if (spawnDelay.isDone()) {
			spawnFloater();
			scale = 2.f;

			spawnCount++;

			spawnDelay.reset();
		}
	}
}

void Spawner::spawnFloater() {
	map->spawnEnemy<Floater>(position);

	sndSpawn.volume = 0.2f + 0.4f * frand();
	sndSpawn.play();
}

void Spawner::draw() {
	pentagramMesh.scale = vec3(6.f - 3.f * beginDelay.value) * pentagramScale;
	pentagramMesh.color.a = 0.5f * (beginDelay.value - endDelay.value);
	pentagramMesh.draw();

	if (beginDelay.value > 0.2f) {
		float time = (beginDelay.value - 0.2f) / 0.8f;
		
		ringMesh.scale = vec3(6.f - 3.f * beginDelay.value) * pentagramScale;
		ringMesh.color.a = beginDelay.value - endDelay.value;
		ringMesh.draw();
	}

	mesh.rotation.z = -rotation;
	mesh.scale = vec3(getSize());

	mesh.color = ColorConfig::getEnemy() * vec4(0.8f, 0.8f, 0.8f, 1.f);
	mesh.draw();

	mesh.rotation.z = rotation;
	mesh.scale = vec3(getSize());

	mesh.color = ColorConfig::getEnemy();
	mesh.draw();
}