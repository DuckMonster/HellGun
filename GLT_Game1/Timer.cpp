#include "Timer.hpp"

#include <math.h>

#include "GameState.hpp"

Timer::Timer(float time) {
	this->time = time;
}

void Timer::update() {
	value = fmin(value + GameState::deltaTime / time, 1.f);
}