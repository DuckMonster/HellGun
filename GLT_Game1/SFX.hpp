#pragma once
#include <SFML\Audio\Sound.hpp>
#include <string>

class SFX {
public:
	float volume = 1.f;

	//-----------------

	SFX() { }
	SFX(std::string filename);
	~SFX();

	void load(std::string filename);
	void play();

private:
	sf::Sound* sound = nullptr;
};