#pragma once
#include "FX.hpp"
#include "Timer.hpp"
#include <glm\glm.hpp>
#include <GLT\MeshDrawer.hpp>
#include <vector>

class FX_Kill : public FX {
public:
	FX_Kill(glm::vec2 position, glm::vec2 direction);

	void logic() override;
	void draw() override;

private:
	struct Smoke {
	public:
		Smoke(glm::vec2 position, float size, float velocity);
		
		void logic();
		void draw();

	private:
		glt::MeshDrawer mesh;

		Timer timer = Timer(0.5f);

		float size;
		glm::vec2 velocity;
	};

	struct Debree {
	public:
		Debree(glm::vec2 position, float direction);

		void logic();
		void draw();

	private:
		glt::MeshDrawer mesh;

		glm::vec2 position;
		glm::vec2 velocity;

		Timer flyTimer = Timer(0.4f);
		Timer smokeTimer = Timer(0.02f);

		std::vector<Smoke> smokeList;
	};

	Timer timer = Timer(1.5f);
	std::vector<Smoke> smokeCloud;
	std::vector<Debree> debreeList;
};