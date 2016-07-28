#include "MathUtil.hpp"

float frand() {
	return (float)rand() / (float)RAND_MAX;
}

float getAngle(glm::vec2 vector) {
	return glm::degrees(atan2(vector.y, vector.x));
}

glm::vec2 getVector(float angle) {
	return glm::vec2(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)));
}

float linToExp(float a) {
	a = fmax(fmin(a, 1.f), 0.f);

	return 1 - exp(-a * 3.f);
}

float angleDif(float a, float b) {
	a = mod(a, -180.f, 180.f);
	b = mod(b, -180.f, 180.f);

	float dif = b - a;
	if (dif > 180)
		dif -= 360.f;
	if (dif < -180)
		dif += 360.f;

	return dif;
}