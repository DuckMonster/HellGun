#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Ray.hpp"

using namespace glm;

namespace glt {
	class Camera {
	public:
		static Camera* active;

		mat4 getMatrix();
		
		vec3 position = vec3(0.f, 0.f, 1.f);
		vec3 direction = vec3(0.f, 0.f, -1.f);

		float fieldOfView = 0.45f;
		float near = 0.1f;
		float far = 100.f;

		bool perspective = true;
		vec2 screenSize = vec2(800.f, 600.f);

		Camera(vec2 screenSize);

		Ray screenToWorld(int x, int y) { return screenToWorld(vec2(x, y)); }
		Ray screenToWorld(vec2 screen);
		vec2 worldToScreen(vec3 world);
		float getAspectRatio() { return screenSize.x / screenSize.y; }

		void setActive();
		void updateShader(GLuint, const char*);

	private:
		mat4 cameraMatrix;

		//Dirtyness for matrix calculation
		bool isDirty;

		vec3 position_old;
		vec3 direction_old;

		float fov_old;
		float near_old;
		float far_old;

		bool perspective_old;
		vec2 screenSize_old;
		//---

		void checkDirty();
		void clean();
	};
}