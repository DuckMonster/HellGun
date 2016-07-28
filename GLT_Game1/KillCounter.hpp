#pragma once
#include <GLT\MeshDrawer.hpp>
#include <GLT\TextMesh.hpp>
#include <glm\vec4.hpp>
#include "Timer.hpp"

class Map;

class KillCounter {
public:
	KillCounter(Map* map);

	void logic();
	void draw();

	void increment();
	unsigned int getScore() { return kills; }

private:
	Map* map;
	int kills = 0;

	glt::TextMesh mesh;
	glt::MeshDrawer drawer;

	Timer animTimer = Timer(0.2f);

	glm::vec4 idleColor = glm::vec4(1.f, 1.f, 1.f, 0.4f);
	glm::vec4 killColor = glm::vec4(1.f, 0.f, 0.f, 1.f);

	//------

	glm::vec4 getColor();
};