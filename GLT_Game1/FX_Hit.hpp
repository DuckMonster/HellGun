#pragma once
#include <GLT\MeshDrawer.hpp>

#include "FX.hpp"

#include "Timer.hpp"

class FX_Hit : public FX {
public:
	FX_Hit(glm::vec2 position, glm::vec2 direction);

	void logic() override;
	void draw() override;

private:
	glt::MeshDrawer mesh;
	glm::vec2 size;

	Timer timer = Timer(0.12f);
};