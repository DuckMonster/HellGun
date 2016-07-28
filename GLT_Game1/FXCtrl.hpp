#pragma once
#include <vector>

class FX;

class FXCtrl {
public:
	static void addFX(FX* fx);
	static void removeFX(size_t index);

	static void clear();

	static void update();
	static void draw();

private:
	static std::vector<FX*> fxList;
};