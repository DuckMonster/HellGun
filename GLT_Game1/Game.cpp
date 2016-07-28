#include "Game.hpp"

#include <GLT\Time.hpp>
#include "AssetBank.hpp"
#include "GameState.hpp"

#include "Entity.hpp"
#include "Player.hpp"

using namespace glt;
using namespace std;

Game* Game::active = nullptr;

Game::Game() : camera(vec2(800, 600)) {
	Game::active = this;
	camera.position = vec3(0.f, 0.f, 50.f);
	camera.far = 10000.f;

	nameTitleBox.font = nameBox.font = nameLengthBox.font = AssetBank::loadFont("basicFont");
	nameTitleBox.text = "Mortal Name:";
	nameTitleBox.scale = vec3(2.f);
	nameTitleBox.color = vec4(1.f, 0.f, 0.f, 1.f),

	nameBox.text = "_";
	nameBox.scale = vec3(1.4f);
	nameBox.color = vec4(0.7f, 0.7f, 0.7f, 1.f);

	nameLengthBox.text = "Name too long!";
	nameLengthBox.scale = vec3(0.8f);
	nameLengthBox.color = vec4(1.f, 0.f, 0.f, 1.f);

	startNewMap();
}

void Game::startNewMap() {
	camera.position = vec3(0.f, 0.f, 20.f);

	if (map != nullptr) {
		delete map;
		map = nullptr;
	}

	map = new Map();
}

void Game::setName(string name) {
	if (name.length() > MAX_NAME_LENGTH) {
		nameLengthError = true;
		nameLengthErrorTimer.reset();

		GameState::keyboardString = name = name.substr(0, MAX_NAME_LENGTH);
	}
	else nameLengthError = false;

	GameState::playerName = name;
}

void Game::logic() {
	if (nameEntered)
		map->logic();
	else {
		nameLengthErrorTimer.update();

		if (GameState::getKeyInput().keys[sf::Keyboard::Return]) {
			nameEntered = true;
		}
	}
}

void Game::draw() {
	camera.setActive();
	map->draw();

	if (!nameEntered) {
		nameTitleBox.position.y = 2.f + 0.1f * sin(Time::total() * 0.9f);
		nameTitleBox.rotation.y = 8.f * sin(Time::total() * 0.5231f);

		nameTitleBox.draw();

		nameBox.text = GameState::playerName + "|";
		nameBox.position.y = 0.1f * sin(Time::total() * 0.9f + 5000);
		nameBox.rotation.y = 8.f * sin(Time::total() * 0.5231f + 5000);
		nameBox.color.a = 1.f + 0.4f * cos(Time::total() * 2.5f);
		nameBox.draw();

		if (nameLengthError) {
			nameLengthBox.position.y = -0.8f + 0.1f * sin(Time::total() * 0.2556f);
			nameLengthBox.rotation.y = 8.f * sin(Time::total() * 0.43215f);
			nameLengthBox.color.a = 1.f - 0.6f * nameLengthErrorTimer.value;

			nameLengthBox.draw();
		}
	}
}