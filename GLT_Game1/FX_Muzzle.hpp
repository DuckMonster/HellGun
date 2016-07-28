#pragma once
#include <glm\glm.hpp>
#include <GLT\MeshDrawer.hpp>
#include <vector>

#include "Timer.hpp"
#include "FX.hpp"

class FX_Muzzle : public FX {
public:
	FX_Muzzle(glm::vec2 position, glm::vec2 direction);
	~FX_Muzzle();

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
	};

	class Spark {
	public:
		Spark(glt::Mesh* mesh, glm::vec2 position, float size, float direction, float velocity, float life);

		void logic();
		void draw();

	private:
		glt::MeshDrawer mesh;

		glm::vec2 position;
		glm::vec2 size;
		float direction;
		float velocity;
		float angularVelocity;
		Timer life = Timer(0.4f);
	};
	
	glt::Mesh smallFlash;
	glt::Mesh bigFlash;

	Timer timer = Timer(0.5f);

	std::vector<Flash*> flashList;
	std::vector<Spark*> sparkList;
};