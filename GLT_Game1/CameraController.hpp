#pragma once
#include <GLT\Camera.hpp>

class Entity;

class CameraController {
public:	
	CameraController() {};
	CameraController(glt::Camera* camera);

	vec3 getTargetPosition();

	void logic();
	void setCamera(glt::Camera* camera) { this->camera = camera; }
	void setFollowEntity(Entity* entity) { followEntity = entity; }

private:
	glt::Camera* camera = nullptr;
	Entity* followEntity = nullptr;
};