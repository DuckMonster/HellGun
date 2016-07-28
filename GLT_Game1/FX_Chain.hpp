#pragma once
#include <glm\glm.hpp>
#include <GLT\MeshDrawer.hpp>
#include <vector>
#include "FX.hpp"

class FX_Chain : public FX {
public:
	static int TEMP;
	glm::vec4 color = glm::vec4(1.f);

	//-----------

	FX_Chain(glm::vec2 start, int segments, float segmentSize);
	~FX_Chain();

	void setRootPosition(glm::vec2 position);

	void logic() override;
	void draw() override;

private:
	class Node {
	public:
		const float NODE_LENGTH = 1.5f;

		Node* child = nullptr, *parent = nullptr;
		glm::vec2 position;

		float size = 0.f;
		float alpha = 0.f;

		//---------

		Node(Node* parent, float size, float alpha);

		void updatePosition();

		void getVertices(float* vertArray, float* colorArray, int index);
		Node* getLast();

	private:
		glm::vec2 directionToParent;
	};

	Node* root;
	glt::MeshDrawer mesh;

	int segments;
};