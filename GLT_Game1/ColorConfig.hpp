#pragma once
#include <vector>
#include <glm\vec4.hpp>

class ColorConfig {
public:
	static void load(const char* filename);

	static glm::vec4 getForeground() { return foregroundColors[level]; }
	static glm::vec4 getBackground() { return backgroundColors[level]; }
	static glm::vec4 getFog() { return fogColors[level]; }

	static glm::vec4 getPlayer() { return playerColor[level]; }
	static glm::vec4 getUI() { return uiColor[level]; }
	static glm::vec4 getEnemy() { return enemyColor[level]; }

	static void setLevel(int level) { ColorConfig::level = level; }

private:
	static int level;

	static std::vector<glm::vec4> foregroundColors;
	static std::vector<glm::vec4> backgroundColors;
	static std::vector<glm::vec4> fogColors;

	static std::vector<glm::vec4> playerColor;
	static std::vector<glm::vec4> uiColor;
	static std::vector<glm::vec4> enemyColor;

	//------------

	static glm::vec4 readColor(int x, int y, int imgWidth, int imgHeight, unsigned char* data);
};