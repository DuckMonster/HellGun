#pragma once
#include <GLT\Camera.hpp>
#include <GLT\TextBox.hpp>
#include <glm\vec2.hpp>
#include "Map.hpp"

class Entity;

class Game {
public:
	static Game* active;

	glt::Camera camera;
	bool nameEntered = false;

	Game();
	void logic();
	void draw();

	void setName(std::string name);

	void startNewMap();

private:
	const int MAX_NAME_LENGTH = 8;

	Map* map = nullptr;

	//Name entering

	glt::TextBox nameTitleBox;
	glt::TextBox nameBox;
	glt::TextBox nameLengthBox;

	bool nameLengthError = false;
	Timer nameLengthErrorTimer = Timer(0.8f);
};