#pragma once
#include <vector>
#include <glm\vec2.hpp>
#include <GLT\MeshDrawer.hpp>
#include "Map.hpp"
#include "Entity.hpp"

#include "FX.hpp"

class FX_Debree : public FX {
public:
	FX_Debree(glm::vec2 position, glm::vec2 direction, float spread, int amount, glm::vec3 color, Map* map);
	~FX_Debree();

	void logic() override;
	void draw() override;

private:
	class Fragment : public Entity {
	public:
		glm::vec3 color;

		//------------

		Fragment(glm::vec2 position, glm::vec2 direction, float size, float time, glm::vec3 color, Map* map);
		~Fragment();

		void logic() override;
		void draw() override;

		void doPhysics() override;

	private:
		const float SPEED = 80.f;

		Timer timer = Timer(0.f);
	};

	std::vector<Fragment*> fragmentList;
	Timer timer = Timer(2.f);
};