#pragma once
struct Timer {
public:
	float value = 0.f;
	float time = 0.f;

	Timer(float time);

	void update();
	bool isDone() { return value >= 1.f; }

	void reset() { value = 0.f; }
	void reset(float time) { reset(); this->time = time; }
};