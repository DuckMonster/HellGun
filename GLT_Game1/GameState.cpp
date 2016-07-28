#include "GameState.hpp"

float GameState::deltaTime = 0.f;
Input GameState::currentInput = Input();
glm::vec2 GameState::screenSize = glm::vec2(0.f);

std::string GameState::keyboardString = "";
std::string GameState::playerName = "";

JoyState GameState::getJoyInput(unsigned int nmbr) {
	return GameState::currentInput.joystick[nmbr];
}

KeyboardState GameState::getKeyInput() {
	return GameState::currentInput.keyboard;
}

MouseState GameState::getMouseInput() {
	return GameState::currentInput.mouse;
}