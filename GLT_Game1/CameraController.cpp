#include "CameraController.hpp"

#include "GameState.hpp"

#include "Entity.hpp"

CameraController::CameraController(glt::Camera* camera) {
	this->camera = camera;
}

vec3 CameraController::getTargetPosition() {
	vec2 mouse = GameState::getMouseInput().world;
	float distance = length(mouse - followEntity->position);

	vec2 positions[] = {
		followEntity->position,
		mouse,
		vec2(0.f)
	};

	float weights[] = {
		1.f,
		0.4f,
		0.7f
	};

	float totalWeight = 0.f;
	vec2 totalPos = vec2(0.f);

	for (int i = 0; i < 3; i++) {
		totalPos += positions[i];
		totalWeight += weights[i];
	}

	totalPos /= totalWeight;

	return vec3(totalPos, 60.f + distance * 0.4f);
}

void CameraController::logic() {
	if (followEntity == nullptr || camera == nullptr)
		return;

	camera->position += (getTargetPosition() - camera->position) * 10.f * GameState::deltaTime;
}