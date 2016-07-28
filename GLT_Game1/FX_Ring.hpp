#pragma once
#include <GLT\MeshDrawer.hpp>
#include <glm\vec2.hpp>

#include "FX.hpp"
#include "Timer.hpp"

class FX_Ring : public FX {
public:
	FX_Ring(glm::vec2 position, float size = 8.f, float time = 2.f);
	~FX_Ring();

	void logic() override;
	void draw() override;

private:
	Timer timer = Timer(2.f);
	glt::MeshDrawer mesh;

	float size = 1.f;
};