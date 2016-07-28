#include "ColorConfig.hpp"
#include <SOIL\SOIL.h>
#include "AssetBank.hpp"

using namespace std;
using namespace glm;

int ColorConfig::level = 0;
vector<vec4> ColorConfig::playerColor;
vector<vec4> ColorConfig::uiColor;
vector<vec4> ColorConfig::enemyColor;

vector<vec4> ColorConfig::foregroundColors;
vector<vec4> ColorConfig::backgroundColors;
vector<vec4> ColorConfig::fogColors;

//Load color config from file
//Rows, in order
//- Player colors
//- UI colors
//- Enemy colors

//- Foreground
//- Background
//- Fog
void ColorConfig::load(const char* filename) {
	int width, height;

	level = 0;

	playerColor.clear();
	uiColor.clear();
	enemyColor.clear();

	foregroundColors.clear();
	backgroundColors.clear();
	fogColors.clear();

	unsigned char* colors = SOIL_load_image(
		std::string(AssetBank::assetRoot + AssetBank::textureRoot + filename).c_str(),
		&width, &height, nullptr, SOIL_LOAD_RGB);

	for (int i = 0; i < width; i++) {
		playerColor.push_back(readColor(i, 0, width, height, colors));
		uiColor.push_back(readColor(i, 1, width, height, colors));
		enemyColor.push_back(readColor(i, 2, width, height, colors));

		foregroundColors.push_back(readColor(i, 3, width, height, colors));
		backgroundColors.push_back(readColor(i, 4, width, height, colors));
		fogColors.push_back(readColor(i, 5, width, height, colors));
	}

	SOIL_free_image_data(colors);
}

vec4 ColorConfig::readColor(int x, int y, int imgWidth, int imgHeight, unsigned char* data) {
	if (x < 0 || x >= imgWidth || y < 0 || y >= imgHeight)
		return vec4(0.f, 0.f, 0.f, 1.f);

	int index = ((y * imgWidth) + x) * 3;
	return vec4(data[index], data[index + 1], data[index + 2], 256) / 256.f;
}