#pragma once
#include "Enemy.hpp"
#include "SFX.hpp"

class Spawner : public Entity {
public:
	int spawnSize;

	//----------

	Spawner(glm::vec2 position, int spawnSize, float beginDelay, Map* map);

	void destroy() override;

	void logic() override;
	void draw() override;

	void spawnFloater();

private:
	const int DEFAULT_SPAWN_SIZE = 6;
	static SFX sndEnter;
	static SFX sndSpawn;

	Timer beginDelay = Timer(2.f);
	Timer endDelay = Timer(1.2f);
	Timer spawnDelay = Timer(0.2f);

	glt::MeshDrawer pentagramMesh;
	glt::MeshDrawer ringMesh;

	int spawnCount = 0;

	float rotation = 0.f;
	float scale = 1.f;
	float pentagramScale = 1.f;

	//--------------

	bool finished() { return spawnCount >= spawnSize; }
	float getSize();
};