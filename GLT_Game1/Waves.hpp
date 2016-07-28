#pragma once
#include <vector>
#include <glm\vec2.hpp>
#include "Timer.hpp"

class Map;

struct WaveEvent {
	friend class WaveControl;
public:
	enum WaveType {
		Spawner,
		Delay,
		LevelUp,
		Yield,
		Unknown
	};

	WaveType type;

	WaveEvent() { type = Unknown; }
	WaveEvent(WaveType type) {
		this->type = type;
	}
};

struct WaveEvent_Spawner : WaveEvent {
	friend class WaveControl;
public:
	int spawnSize;
	float initDelay;
	glm::vec2 position;

	WaveEvent_Spawner() {}
	WaveEvent_Spawner(std::string str) : WaveEvent(Spawner) {
		parse(str);
	}

private:
	void parse(std::string str);
};

struct WaveEvent_Delay : WaveEvent {
	friend class WaveControl;
public:
	float delayTime;

	WaveEvent_Delay(std::string str) : WaveEvent(Delay) {
		parse(str);
	}

private:
	void parse(std::string str);
};

struct WaveEvent_LevelUp : WaveEvent {
	friend class WaveControl;
public:
	WaveEvent_LevelUp() : WaveEvent(LevelUp) {

	}

private:

};

struct WaveEvent_Yield : WaveEvent {
	friend class WaveControl;
public:
	WaveEvent_Yield() : WaveEvent(Yield) {

	}
};

class WaveControl {
public:
	WaveControl(Map* map);
	~WaveControl();

	void parse(const char* filename);
	void logic();

	void levelCleared();

private:
	Map* map;
	std::vector<WaveEvent*> eventList;

	Timer delayTimer = Timer(0.f);
	bool yielding = false;

	int eventIndex = 0;

	//-------------

	void addEvent(WaveEvent* wave);

	WaveEvent* getCurrentEvent() {
		return eventList[eventIndex];
	}
};