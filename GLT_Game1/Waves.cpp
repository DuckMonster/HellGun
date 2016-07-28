#include "Waves.hpp"

#include <fstream>
#include <string>
#include "Floater_Spawner.hpp"
#include "StringUtils.h"
#include "MathUtil.hpp"

#include "Map.hpp"

using namespace std;
using namespace glm;

WaveControl::WaveControl(Map* map) {
	this->map = map;
}

WaveControl::~WaveControl() {
	for (int i = 0; i < eventList.size(); i++)
		delete eventList[i];
}

void WaveControl::levelCleared() {
	//delayTimer.reset(0.f);
	delayTimer.value = 1.f;
	yielding = false;
}

void WaveControl::logic() {
	if (yielding)
		return;

	delayTimer.update();

	while (delayTimer.isDone()) {
		//If event index exceeds event count, advance the wave
		if (eventIndex >= eventList.size())
			eventIndex = 0;

		//Event time
		WaveEvent* current = getCurrentEvent();

		//Advance event
		eventIndex++;

		//Spawner
		if (current->type == WaveEvent::Spawner) {
			//Cast to spawner event
			WaveEvent_Spawner* spawnerEvent = (WaveEvent_Spawner*)current;

			//Create the spawner object
			Spawner* spawner = new Spawner(spawnerEvent->position * map->getSize(), spawnerEvent->spawnSize, spawnerEvent->initDelay, map);

			//Add to world
			map->addEntity(spawner);

			std::cout << "Event: spawner\n";
		}

		//Delay
		else if (current->type == WaveEvent::Delay) {
			//Cast to delay event
			WaveEvent_Delay* delayEvent = (WaveEvent_Delay*)current;

			//Reset delay time
			delayTimer.reset(delayEvent->delayTime);

			std::cout << "Event: delay\n";
		}

		//Yielding
		else if (current->type == WaveEvent::Yield) {
			yielding = true;
			std::cout << "Event: yield\n";

			break;
		}

		else if (current->type == WaveEvent::LevelUp) {
			map->levelUp();

			std::cout << "LEVEL UP!\n";
		}
	}
}

void WaveControl::addEvent(WaveEvent* event) {
	eventList.push_back(event);
}

void WaveControl::parse(const char* filename) {
	ifstream stream;
	stream.open(filename);

	if (stream.is_open()) {
		string line;

		while (getline(stream, line, '\n')) {
			line = trim(line, string("\t"));

			//Spawner
			if (line[0] == '#')
				addEvent(new WaveEvent_Spawner(line));

			//Delay
			else if (line[0] == '$')
				addEvent(new WaveEvent_Delay(line));

			//Levelup
			else if (line[0] == '!')
				addEvent(new WaveEvent_LevelUp());

			//Yield
			else if (line[0] == '?')
				addEvent(new WaveEvent_Yield());
		}
	}
}

//-----

void WaveEvent_Spawner::parse(string s) {
	vector<string> params = split(s, ' ');

	//Position
	vector<string> position = split(params[1], ':');
	float x, y;

	if (position[0] == "r")
		x = (frand() - 0.5f) * 0.95f;
	else
		x = stof(position[0]);

	if (position[1] == "r")
		y = (frand() - 0.5f) * 0.95f;
	else
		y = stof(position[1]);

	//Spawn delay and size
	vector<string> spawn = split(params[2], ':');
	float initDelay = stof(spawn[0]);
	int spawnSize = stoi(spawn[1]);

	this->position = vec2(x, y);
	this->initDelay = initDelay;
	this->spawnSize = spawnSize;
}

void WaveEvent_Delay::parse(string s) {
	vector<string> params = split(s, ' ');

	this->delayTime = stof(params[1]);
} 