#pragma once
#include <glm\glm.hpp>
#include <GLT\MeshDrawer.hpp>
#include <vector>

#include "Timer.hpp"
#include "FX.hpp"

class FX_Jump_Ground : public FX {
public:
	FX_Jump_Ground(glm::vec2 position);
	~FX_Jump_Ground();

	void logic() override;
	void draw() override;

private:
	class Smoke {
	public:
		Smoke(glm::vec2 position, float direction, float velocity, float size, float life);

		void logic();
		void draw();

	private:
		glm::vec2 position;
		float direction;
		float velocity;
		float size;
		Timer life = Timer(0.5f);

		glt::MeshDrawer mesh;
	};
	 
	std::vector<Smoke*> smokeList;

	Timer timer = Timer(1.f);

	glt::MeshDrawer lineMesh;
};