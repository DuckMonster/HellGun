#pragma once
#include "Actor.hpp"
#include <SFML\Audio.hpp>

class Player;

class Enemy : public Actor {
public:
	Enemy(glm::vec2 position, Player* player, Map* map);

	void logic() override;
	void hit(glm::vec2 direction);

	bool isAlive() { return health > 0; }

protected:
	static SFX* sndDie;

	Player* target;
	int health = 1;
};