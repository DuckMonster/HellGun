#include "Bullet.hpp"

#include <glm/vec2.hpp>
#include "FX_Ring.hpp"
#include "MathUtil.hpp"
#include "GameState.hpp"

#include "Gun.hpp"
#include "BulletPickup.hpp"
#include "Enemy.hpp"

using namespace glm;

Bullet::Bullet(Gun* host, vec2 direction, Map* map) : Entity(map) {
	this->host = host;
	position = host->position + direction * BULLET_OFFSET;
	velocity = direction * BULLET_VELOCITY;
	size = vec2(0.4f);

	trail = new FX_Trail(this, 1.5f, 0.5f, 0.05f);
}

void Bullet::logic() {
	RayHit<Enemy> hit = map->raytrace<Enemy>(AABB::fromPositionSize(vec2(0.f), size), position, position + velocity * GameState::deltaTime);

	position = hit.location;
	if (hit.hit) {
		destroy();

		if (hit.entity != nullptr) {
			//Try casting to enemy pointer
			Enemy* asEnemy = dynamic_cast<Enemy*>(hit.entity);

			//If it failed, it is not a enemy
			//Otherwise hit that enemy
			if (asEnemy != nullptr)
				asEnemy->hit(velocity);
		}

		map->addEntity(new BulletPickup(position, normalize(-velocity), host->owner, map));
		new FX_Ring(position, 1.f + 1.f * frand(), 0.1f + frand() * 0.2f);
		trail->release();
	}
}