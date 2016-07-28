#include "Map.hpp"

#include <stdlib.h>
#include <SOIL\SOIL.h>
#include <glm\gtc\type_ptr.hpp>
#include "Game.hpp"
#include "FXCtrl.hpp"
#include "GameState.hpp"
#include "AssetBank.hpp"
#include "ColorConfig.hpp"

#include "Entity.hpp"
#include "Enemy.hpp"
#include "Charger.hpp"
#include "Player.hpp"
#include "Floater.hpp"
#include "Floater_Spawner.hpp"
#include "MathUtil.hpp"

using namespace glm;
using namespace std;
using namespace glt;

Map::Map() : killCounter(this), waveControl(this), mapVisuals(this) {
	//Color scheme
	ColorConfig::load("defaultColors.png");

	//Clear all FX
	FXCtrl::clear();

	//Create player
	Player* p = new Player(this);
	p->position = glm::vec2(0.f, -5.f);
	addEntity(p);

	//Fade mesh
	fadeMesh.setMesh(AssetBank::loadMesh("plane"));
	fadeMesh.scale = vec3(500.f);
	fadeMesh.color = vec4(0.f, 0.f, 0.f, 1.f);

	//Init cameracontroller
	cameraController.setCamera(&Game::active->camera);
	cameraController.setFollowEntity(p);

	waveControl.parse("waveInfo.txt");
}

Map::~Map() {
	while (entityList.size() > 0)
		deleteEntity(0);
}

vec2 Map::getSize() {
	return mapBox.size();
}

Player* Map::getPlayer() {
	for (int i = 0; i < entityList.size(); i++) {
		Player* curr = dynamic_cast<Player*>(entityList[i]);

		if (curr != nullptr)
			return curr;
	}

	return nullptr;
}

int Map::getEnemyCount() {
	int n = 0;

	for (int i = 0; i < entityList.size(); i++) {
		if (dynamic_cast<Enemy*>(entityList[i]) || dynamic_cast<Spawner*>(entityList[i]))
			n++;
	}

	return n;
}

void Map::playerDie() {
	scoreboard.setScore(killCounter.getScore());
	scoreboard.loadOnline();
	gameOver = true;
}

void Map::enemyKilled(bool valued) {
	if (valued)
		killCounter.increment();

	if (getEnemyCount() <= 1)
		waveControl.levelCleared();
}

void Map::levelUp() {
	currentLevel++;
	ColorConfig::setLevel(currentLevel);
}

void Map::addEntity(Entity* ptr) {
	entityList.push_back(ptr);
}

void Map::deleteEntity(size_t index) {
	if (index >= entityList.size())
		return;

	delete entityList[index];
	entityList.erase(entityList.begin() + index);
}

void Map::logic() {
	deltaBuffer = GameState::deltaTime;
	fadeTimer.update();

	if (gameOver) {
		dieTimer.update();

		if (dieTimer.isDone()) {
			if (GameState::getMouseInput().buttons[sf::Mouse::Left]) {
				Game::active->startNewMap();
				return;
			}
		}

		GameState::deltaTime *= 0.1f + 0.9f * (1.f - dieTimer.value);
	}

	std::vector<size_t> destroyedList;

	for (size_t i = 0; i < entityList.size(); i++) {
		entityList[i]->logic();

		//Check if destroyed
		if (entityList[i]->isDestroyed())
			destroyedList.push_back(i);
	}

	//Iterate through the destroyed-list backwards (so that the indices are correct)
	for (int i = destroyedList.size() - 1; i >= 0; i--)
		deleteEntity(destroyedList[i]);

	//Spawn enemies
	waveControl.logic();

	//Move camera
	cameraController.logic();

	//Update killcounter
	killCounter.logic();

	//Update map visuals
	mapVisuals.logic();

	//Update fx
	FXCtrl::update();
}

bool Map::getCollision(const AABB& boundingBox) {
	//Map walls
	if (!mapBox.encapsules(boundingBox))
		return true;

	return false;
}

RayHit<Entity> Map::raytrace(const AABB& boundingBox, glm::vec2 start, glm::vec2 end) {
	//Divide the distance into segments
	//Segment length is the smallest of width or height of AABB
	//This is to ensure an interconnected line is covered
	int segments = ceil(length(end - start) / fmax(boundingBox.size().x, boundingBox.size().y));

	vec2 step = (end - start) / (float)segments;

	vec2 prevPos = start;
	//Then go through all the segments!
	for (int i = 0; i <= segments; i++) {
		//Current checking
		vec2 pos = start + step * (float)i;

		//Offset the bounding box!
		AABB currentBox = boundingBox.offset(pos);

		//Or if it's outside the map :P
		if (!mapBox.encapsules(currentBox)) {
			return RayHit<Entity>(true, prevPos, nullptr);
		}

		prevPos = pos;
	}

	//Nothing hit
	return RayHit<Entity>(false, end, nullptr);
}

void Map::draw() {
	mapVisuals.draw();

	killCounter.draw();

	for (std::vector<Entity*>::iterator p = entityList.begin(); p != entityList.end(); p++)
		(*p)->draw();

	FXCtrl::draw();

	getPlayer()->drawUI();

	GameState::deltaTime = deltaBuffer;

	fadeMesh.color.a = 1.f - fadeTimer.value;
	fadeMesh.draw();

	//Draw scoreboard if game over
	if (gameOver) {
		fadeMesh.color = vec4(1.f, 0.f, 0.f, 0.4f);
		fadeMesh.draw();

		scoreboard.draw();
	}
}