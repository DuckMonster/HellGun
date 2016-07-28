#pragma once
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>

float frand();
float getAngle(glm::vec2 vector);
glm::vec2 getVector(float angle);
float linToExp(float a);

float angleDif(float a, float b);

template<class T> T mod(T value, T min, T max) {
	T span = max - min;
	while (value >= max) value -= span;
	while (value < min) value += span;

	return value;
}