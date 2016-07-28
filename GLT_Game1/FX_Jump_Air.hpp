#pragma once
#include <glm\glm.hpp>
#include <GLT\MeshDrawer.hpp>
#include <vector>

#include "Timer.hpp"
#include "FX.hpp"

class Player;

class FX_Jump_Air : public FX {
public:
	FX_Jump_Air(glm::vec2 position, glm::vec2 direction, Player* player);
	~FX_Jump_Air();

	void logic() override;
	void draw() override;

private:
	class Flash {
	public:
		Flash(glt::Mesh* mesh, glm::vec2 position, float direction, glm::vec2 size, float time);

		void logic();
		void draw();

	private:
		glt::MeshDrawer mesh;
		glm::vec2 size;
		Timer life = Timer(0.1f);

		glm::vec2 direction;
	};

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

	Player* player;

	glm::vec2 position;
	glm::vec2 direction;

	std::vector<Flash*> flashList;
	std::vector<Smoke*> smokeList;

	Timer timer = Timer(1.f);
	Timer lineTimer = Timer(0.12f);
	Timer smokeSpawnTimer = Timer(0.1f);

	glt::Mesh flashMesh;
	glt::MeshDrawer lineMesh;
};