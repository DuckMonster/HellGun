#include "Floater.hpp"

#include "GameState.hpp"
#include "MathUtil.hpp"
#include "AssetBank.hpp"
#include "ColorConfig.hpp"
#include <GLT\Mesh.hpp>

#include "Player.hpp"

Floater::Floater(glm::vec2 position, Player* target, Map* map) : Enemy(position, target, map) {
	health = 1;
	size = vec2(1.2f);

	float uvs[] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,

		0.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
	};

	mesh.setMesh(AssetBank::loadMesh("plane"));
	mesh.getMesh()->setUVS(uvs, sizeof(uvs));
	mesh.texture = AssetBank::loadTexture("floater");

	upperMesh.setMesh(AssetBank::loadMesh("floater", 0));
	lowerMesh.setMesh(AssetBank::loadMesh("floater", 1));
	bodyMesh.setMesh(AssetBank::loadMesh("floater", 2));

	size = vec2(1.8f);
	upperMesh.scale = lowerMesh.scale = bodyMesh.scale = vec3(size, 1.f);
	scaleMesh = true;

	direction = (frand() - 0.5f) * 360.f;
	speedFactor = 1.f + (frand() - 0.5f) * 0.2f;

	chain[0] = new FX_Chain(position, 15, 0.5f);
	chain[1] = new FX_Chain(position, 2 + (int)(10 * frand()), 0.3f);
	chain[2] = new FX_Chain(position, 2 + (int)(10 * frand()), 0.3f);

	for (int i = 0; i < 3; i++)
		chain[i]->color = vec4(1.f, 0.f, 0.f, 1.f);
}

Floater::~Floater() {
	Entity::~Entity();
	chain[0]->finish();
	chain[1]->finish();
	chain[2]->finish();
}

void Floater::logic() {
	driftValue += GameState::deltaTime * frand();
	biteValue += (getTargetBite() - biteValue) * 2.f * GameState::deltaTime;

	float twitchAmount = (frand() - 0.5f) * 30.f;
	twitchAmount = 5.f * sign(twitchAmount) + twitchAmount;

	float targetBite = getTargetBite();

	biteValue = clamp(biteValue + twitchAmount * GameState::deltaTime, targetBite * 0.8f, 0.5f + 0.5f * targetBite);

	updateDirection();
	position += getVector(direction + getDrift()) * FLOAT_SPEED * speedFactor * GameState::deltaTime;

	vec2 normal = getVector(direction + getDrift());
	normal = vec2(-normal.y, normal.x) * 0.6f * glm::sin(glm::radians(direction * 5.f));

	chainUpdateTimer.update();
	if (chainUpdateTimer.isDone()) {
		chain[0]->setRootPosition(position);
		chain[1]->setRootPosition(position + normal);
		chain[2]->setRootPosition(position - normal);
		chainUpdateTimer.reset();
	}

	animBegin.update();

	Enemy::logic();
}

void Floater::updateDirection() {
	float deltaAngle = angleDif(direction, getAngle(target->position - position));
	deltaAngle = clamp(deltaAngle, -TURN_SPEED * GameState::deltaTime, TURN_SPEED * GameState::deltaTime);

	direction += deltaAngle;
	//mesh.rotation.x += deltaAngle * 5.1f;
}

float Floater::getDrift() {
	return sin(driftValue * 4.f) * 20.f;
}

float Floater::getTargetBite() {
	float angleToPlayer = getAngle(target->position - position) - mod(direction + getDrift(), -180.f, 180.f);
	float distanceToPlayer = length(target->position - position);

	float targetBite = 0.f;
	
	//Begin animation
	targetBite += 1.f - animBegin.value;

	if (abs(angleToPlayer) <= 45.f && distanceToPlayer <= 20.f)
		targetBite += (1.f - distanceToPlayer / 20.f) * (1.f - abs(angleToPlayer) / 45.f);

	return targetBite;
}

void Floater::draw() {
	upperMesh.color = lowerMesh.color = bodyMesh.color = ColorConfig::getEnemy();
	upperMesh.rotation.z = lowerMesh.rotation.z = bodyMesh.rotation.z = direction + getDrift();

	upperMesh.rotation.z += biteValue * BITE_ANGLE;
	lowerMesh.rotation.z -= biteValue * BITE_ANGLE;

	upperMesh.position = lowerMesh.position = bodyMesh.position = vec3(position, 0.f);
	
	upperMesh.draw();
	lowerMesh.draw();
	bodyMesh.draw();
}