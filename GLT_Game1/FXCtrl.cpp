#include "FXCtrl.hpp"

#include "FX.hpp"

using namespace std;

vector<FX*> FXCtrl::fxList;

void FXCtrl::addFX(FX* fx) {
	fxList.push_back(fx);
}

void FXCtrl::removeFX(size_t index) {
	if (index >= fxList.size())
		return;

	delete fxList[index];
	fxList.erase(fxList.begin() + index);
}

void FXCtrl::clear() {
	while (fxList.size() > 0)
		removeFX(0);
}

void FXCtrl::update() {
	for (int i = 0; i < fxList.size(); i++) {
		fxList[i]->logic();

		//Remove FX if done
		if (fxList[i]->isFinished()) {
			removeFX(i);

			//Decrease incrementor to compensate for removal of effect
			i--;
		}
	}
}

void FXCtrl::draw() {
	for (int i = 0; i < fxList.size(); i++)
		fxList[i]->draw();
}