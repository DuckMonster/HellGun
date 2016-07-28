#include "FX_Chain.hpp"

#include "MathUtil.hpp"

using namespace glm;
using namespace glt;
using namespace std;

int FX_Chain::TEMP = 0;

FX_Chain::FX_Chain(vec2 start, int segments, float segmentSize) : FX() {
	this->segments = segments;

	mesh.setMesh(new Mesh());
	mesh.getMesh()->drawMode = GL_QUADS;

	root = new Node(nullptr, segmentSize, 1.f);
	root->position = start;
	Node* curr = root;

	float value = 1.f;

	for (int i = 0; i < segments; i++) {
		value *= 0.8f + frand() * 0.2f;

		float size = segmentSize * pow(0.8f, i);

		if (i == segments - 1)
			size = 0.f;

		curr = new Node(curr, size, value);
	}
}

FX_Chain::~FX_Chain() {
	FX::~FX();

	Node* curr = root->getLast();
	do {
		Node* next = curr->parent;
		delete curr;

		curr = next;
	} while (curr != nullptr);

	delete mesh.getMesh();
}

void FX_Chain::setRootPosition(glm::vec2 position) {
	root->position = position;
	root->updatePosition();

	float* vertices = new float[segments * 4 * 3];
	float* colors = new float[segments * 4 * 4];

	root->getVertices(vertices, colors, 0);
	mesh.getMesh()->setVertices(vertices, segments * 4 * 3 * sizeof(float));
	mesh.getMesh()->setColors(colors, segments * 4 * 4 * sizeof(float));

	delete[] vertices;
	delete[] colors;
}

void FX_Chain::logic() {
	FX::logic();
}

void FX_Chain::draw() {
	FX::draw();

	mesh.color = color;
	mesh.draw();
}

FX_Chain::Node::Node(Node* parent, float size, float alpha) {
	this->parent = parent;

	if (parent != nullptr) {
		parent->child = this;
		position = parent->position;
	}

	this->size = size;
	this->alpha = alpha;
}

void FX_Chain::Node::updatePosition() {
	if (parent != nullptr) {
		vec2 dif = parent->position - position;
		float distance = sqrt(dif.x * dif.x + dif.y * dif.y);
		directionToParent = dif / distance;

		if (distance > NODE_LENGTH)
			position = parent->position + -directionToParent * NODE_LENGTH;
	}

	if (child != nullptr)
		child->updatePosition();
}

FX_Chain::Node* FX_Chain::Node::getLast() {
	Node* current = this;

	while (current->child != nullptr) {
		current = current->child;
	}

	return current;
}


void FX_Chain::Node::getVertices(float* vertArray, float* colorArray, int index) {
	if (child == nullptr)
		return;

	int vi = index * 3 * 4;
	int ci = index * 4 * 4;

	glm::vec2 norm = child->directionToParent;
	norm = glm::vec2(-norm.y, norm.x);

	glm::vec2 myNorm = norm * size;
	glm::vec2 childNorm = norm * child->size;

	glm::vec4 myColor = vec4(alpha, alpha, alpha, 1.f);
	glm::vec4 childColor = vec4(child->alpha, child->alpha, child->alpha, 1.f);

	vertArray[vi + 0] = position.x + myNorm.x;
	vertArray[vi + 1] = position.y + myNorm.y;
	vertArray[vi + 2] = 0.f;

	vertArray[vi + 3] = position.x - myNorm.x;
	vertArray[vi + 4] = position.y - myNorm.y;
	vertArray[vi + 5] = 0.f;

	vertArray[vi + 6] = child->position.x - childNorm.x;
	vertArray[vi + 7] = child->position.y - childNorm.y;
	vertArray[vi + 8] = 0.f;

	vertArray[vi + 9] = child->position.x + childNorm.x;
	vertArray[vi + 10] = child->position.y + childNorm.y;
	vertArray[vi + 11] = 0.f;

	colorArray[ci + 0] = myColor.r;
	colorArray[ci + 1] = myColor.g;
	colorArray[ci + 2] = myColor.b;
	colorArray[ci + 3] = myColor.a;

	colorArray[ci + 4] = myColor.r;
	colorArray[ci + 5] = myColor.g;
	colorArray[ci + 6] = myColor.b;
	colorArray[ci + 7] = myColor.a;

	colorArray[ci + 8] = childColor.r;
	colorArray[ci + 9] = childColor.g;
	colorArray[ci + 10] = childColor.b;
	colorArray[ci + 11] = childColor.a;

	colorArray[ci + 12] = childColor.r;
	colorArray[ci + 13] = childColor.g;
	colorArray[ci + 14] = childColor.b;
	colorArray[ci + 15] = childColor.a;

	child->getVertices(vertArray, colorArray, index + 1);
}