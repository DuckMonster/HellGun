#pragma once
#include <vector>
#include <glm\vec2.hpp>
#include <GLT\MeshDrawer.hpp>
#include <GLT\Shader.hpp>
#include <GLT\TextMesh.hpp>

#include "MapVisuals.hpp"
#include "Collision.hpp"
#include "Timer.hpp"
#include "CameraController.hpp"
#include "KillCounter.hpp"
#include "ScoreBoard.hpp"

#include "Waves.hpp"

class Entity;
class Player;

template<class T>
struct RayHit {
	bool hit;
	glm::vec2 location;
	T* entity;

	RayHit(bool hit, glm::vec2 location, T* entity) {
		this->hit = hit;
		this->location = location;
		this->entity = entity;
	}
};

class Map {
public:
	Map();
	~Map();

	void logic();
	void draw();

	glm::vec2 getSize();

	void playerDie();

	void addEntity(Entity* ptr);
	void deleteEntity(size_t index);

	void enemyKilled(bool valued);
	void levelUp();

	Player* getPlayer();
	int getEnemyCount();

	bool getGameOver() { return gameOver; }

	bool getCollision(const AABB& boundingBox);
	template<class T>
	bool getCollision(const AABB& boundingBox, T* ignore = nullptr, T** hitEntity = nullptr);

	RayHit<Entity> raytrace(const AABB& boundingBox, glm::vec2 start, glm::vec2 end);

	template<class T>
	RayHit<T> raytrace(const AABB& boundingBox, glm::vec2 start, glm::vec2 end, T* ignore = nullptr);

	//Spawning enemies
	template <class T> T* spawnEnemy();
	template <class T> T* spawnEnemy(glm::vec2 position);

protected:
	std::vector<Entity*> entityList;
	AABB mapBox = AABB::fromPositionSize(glm::vec2(0.f), glm::vec2(50.f, 30.f));

	WaveControl waveControl;
	int currentLevel = 0;

	KillCounter killCounter;
	ScoreBoard scoreboard;

	//Graphics
	MapVisuals mapVisuals;
	glt::MeshDrawer fadeMesh;

	Timer spawnTimer = Timer(1.f);
	Timer dieTimer = Timer(1.9f);
	Timer fadeTimer = Timer(0.5f);

	CameraController cameraController;

	float deltaBuffer;

	bool gameOver = false;

	//--------
};

#pragma region
////
////
////TEMPLATE DEFINITIONS
////
////

template<typename T>
bool Map::getCollision(const AABB& boundingBox, T* ignore, T** hitEntity) {
	//Map walls
	if (!mapBox.encapsules(boundingBox))
		return true;

	for (int i = 0; i < entityList.size(); i++) {
		if (dynamic_cast<T*>(entityList[i]) == nullptr || entityList[i] == ignore || !entityList[i]->collisionEnabled) continue;

		if (intersects(entityList[i]->getBoundingBox(), boundingBox)) {
			if (hitEntity != nullptr)
				*hitEntity = entityList[i];

			return true;
		}
	}

	return false;
}

template <class T>
RayHit<T> Map::raytrace(const AABB& boundingBox, glm::vec2 start, glm::vec2 end, T* ignore) {
	//Populate a list with entities that actually are within the raytrace box
	std::vector<Entity*> rayEntityList;
	AABB rayBox(start, end);
	rayBox = AABB(rayBox.min() - boundingBox.size() / 2.f, rayBox.max() + boundingBox.size() / 2.f);

	for (int i = 0; i < entityList.size(); i++) {
		if (dynamic_cast<T*>(entityList[i]) != nullptr &&
			entityList[i] != ignore &&
			entityList[i]->collisionEnabled &&
			intersects(entityList[i]->getBoundingBox(), rayBox))
			rayEntityList.push_back(entityList[i]);
	}
	//

	//Divide the distance into segments
	//Segment length is the smallest of width or height of AABB
	//This is to ensure an interconnected line is covered
	int segments = ceil(length(end - start) / fmax(boundingBox.size().x, boundingBox.size().y));

	vec2 prevPos = start;
	//Then go through all the segments!
	for (int i = 0; i <= segments; i++) {
		//Current checking
		vec2 pos = start + ((end - start) / (float)segments) * (float)i;

		//Offset the bounding box!
		AABB currentBox = boundingBox.offset(pos);

		//Check all the entities added within the ray AABB
		for (int j = 0; j < rayEntityList.size(); j++) {
			if (intersects(currentBox, rayEntityList[j]->getBoundingBox())) {
				return RayHit<T>(true, prevPos, dynamic_cast<T*>(rayEntityList[j]));
			}
		}

		//Or if it's outside the map :P
		if (!mapBox.encapsules(currentBox))
			return RayHit<T>(true, prevPos, nullptr);

		prevPos = pos;
	}

	//Nothing hit
	return RayHit<T>(false, end, nullptr);
}

template<class T> T* Map::spawnEnemy() {
	return spawnEnemy<T>(getVector(frand() * 180.f) * 15.f);
}

template<class T> T* Map::spawnEnemy(glm::vec2 position) {
	T* newActor = new T(position, getPlayer(), this);

	addEntity(newActor);
	return newActor;
}
#pragma endregion Template definitions