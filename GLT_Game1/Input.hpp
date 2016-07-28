#pragma once
#include <SFML\Window\Joystick.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include <glm\vec2.hpp>
#include <array>

struct JoyState {
	std::array<bool, sf::Joystick::ButtonCount> buttons;
	std::array<float, sf::Joystick::AxisCount> axes;

	bool getButton(unsigned int button) { return buttons[button]; }
	float getAxis(unsigned int axis) { return axes[(int)axis]; }
	glm::vec2 getAxis(unsigned int x, unsigned int y) { return glm::vec2(axes[(int)x], axes[(int)y]); }
};

struct KeyboardState {
	std::array<bool, sf::Keyboard::KeyCount> keys;
	bool getKey(unsigned int key) { return keys[key]; }
};

struct MouseState {
	std::array<bool, sf::Mouse::ButtonCount> buttons;
	glm::vec2 world;
	glm::vec2 screen;
	bool getKey(unsigned int key) { return buttons[key]; }
};

struct Input {
	Input() {}
	JoyState joystick[4];
	KeyboardState keyboard;
	MouseState mouse;
};