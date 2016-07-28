#pragma once
#include<glm\glm.hpp>

namespace glt {
	struct Ray {
	public:
		//Creation
		static Ray fromTo(glm::vec3 a, glm::vec3 b);

		//
		glm::vec3 start, direction;

		Ray(glm::vec3, glm::vec3);
		glm::vec3 overlapZ();
		glm::vec3 overlapPlane(glm::vec3, glm::vec3);

		glm::vec3 operator*(float f);
	};
}