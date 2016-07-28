#pragma once
#include <glm\vec2.hpp>
#include <GLT\FrameBuffer.hpp>
#include "Input.hpp"

class GameState {
public:
	static float deltaTime;
	static Input currentInput;
	static glm::vec2 screenSize;
	static std::string keyboardString;
	static std::string playerName;

	static JoyState getJoyInput(unsigned int nmbr);
	static KeyboardState getKeyInput();
	static MouseState getMouseInput();
};