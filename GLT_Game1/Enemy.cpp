#include "Enemy.hpp"

#include "AssetBank.hpp"
#include "MathUtil.hpp"
#include "GameState.hpp"
#include "FX_Ring.hpp"
#include "FX_Debree.hpp"
#include "FX_Hit.hpp"
#include "FX_Kill.hpp"
#include "ColorConfig.hpp"
#include "SoundConfig.hpp"

#include "Player.hpp"

SFX* Enemy::sndDie = nullptr;

Enemy::Enemy(glm::vec2 position, Player* player, Map* map) : Actor(map) {
	if (sndDie == nullptr) {
		sndDie = new SFX[5];
		sndDie[0].load("Kill01");
		sndDie[1].load("Kill02");
		sndDie[2].load("Kill03");
		sndDie[3].load("Kill04");
		sndDie[4].load("Kill05");
	}

	mesh.color = ColorConfig::getEnemy();
	target = player;
	this->position = position;
}

void Enemy::logic() {
	if (getBoundingBox().intersects(target->getBoundingBox())) {
		target->destroy();
	}
	//Entity::logic();
}

void Enemy::hit(vec2 direction) {
	health--;

	if (!isAlive()) {
		int dieSoundIndex = rand() % 5;

		sndDie[dieSoundIndex].volume = 0.4f + 0.3f * frand();
		sndDie[dieSoundIndex].play();

		new FX_Ring(position, 4.f, 0.6f);
		new FX_Kill(position, direction);
		destroy();
		map->enemyKilled(true);
	}
	else {
		new FX_Ring(position, 4.f, 0.2f);
		//new FX_Debree(position, normalize(direction), 45.f, 2, vec3(1.f, 0.f, 0.f), map);
	}

	new FX_Hit(position, direction);
}