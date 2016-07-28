#pragma once
#include <GLT\MeshDrawer.hpp>
#include <glm\vec2.hpp>
#include <vector>

#include "FX.hpp"
#include "Entity.hpp"

class FX_Trail : public FX {
public:
	float minDistance = 1.5f;
	float size = 1.f;
	float segmentLife = 1.f;

	//-----------

	FX_Trail(Entity* target, float minDistance = 1.5f, float size = 1.f, float segmentLife = 1.f);
	~FX_Trail();

	void logic() override;
	void draw() override;

	void release();

private:
	struct Segment {
	public:
		glm::vec2 position;
		glm::vec2 direction;

		//------------

		Segment(glm::vec2 position, glm::vec2 direction, float size = 1.f, float time = 0.2f);

		void update();
		std::array<float, 6> getVertices();
		bool isDone() { return timer.isDone(); }

	private:
		float size = 1.f;
		Timer timer = Timer(0.2f);

		//--------------

		float getSize();
	};

	Entity* target;
	glm::vec2 position;

	std::vector<float> vertices;
	std::vector<Segment> segments;
	glt::MeshDrawer mesh;

	//--------------

	void addSegment(glm::vec2 location, glm::vec2 normal);
	void updateMesh();
};