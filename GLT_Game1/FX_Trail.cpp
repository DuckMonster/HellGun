#include "FX_Trail.hpp"

#include "GameState.hpp"

using namespace glm;
using namespace std;
using namespace glt;

FX_Trail::FX_Trail(Entity* target, float minDistance, float size, float segmentLife) : mesh(new Mesh()), FX() {
	this->target = target;
	this->minDistance = minDistance;
	this->size = size;
	this->segmentLife = segmentLife;

	position = target->position;

	mesh.getMesh()->drawMode = GL_TRIANGLE_STRIP;
	mesh.getMesh()->setVertices(nullptr, 0);
	mesh.getMesh()->setDataUsage(GL_STREAM_DRAW);
}
FX_Trail::~FX_Trail() {
	FX::~FX();
	delete mesh.getMesh();
}

void FX_Trail::addSegment(vec2 position, vec2 normal) {
	segments.push_back(Segment(position, normal, size, segmentLife));
}

void FX_Trail::updateMesh() {
	if (segments.size() <= 0)
		return;

	vector<float> vertexList(segments.size() * 6);
	int index = 0;

	for (vector<Segment>::iterator i = segments.begin(); i != segments.end(); i++) {
		if (i->isDone()) continue;

		array<float, 6> segVertices = i->getVertices();
		for (int j = 0; j < segVertices.size(); j++)
			vertexList[index + j] = segVertices[j];

		index += 6;
	}

	mesh.getMesh()->setVertices(&vertexList[0], vertexList.size() * sizeof(float));
}

void FX_Trail::release() {
	target = nullptr;
}

void FX_Trail::logic() {
	FX::logic();

	if (target != nullptr && !target->isDestroyed()) {
		//Check to add segments
		float distance = length(position - target->position);

		if (distance >= minDistance) { //Minimum
			//Create more segments if needed
			int segments = floor(distance / minDistance);

			vec2 normal = normalize(target->position - position);

			//Create them all, evenly spaced
			for (int i = 0; i < segments; i++) {
				addSegment(target->position - normal * distance * ((float)i / segments), normal);
			}

			//Update position
			position = target->position;
		}
	}
	//If entity is null, and all segments are gone, finish the particle
	else if (segments.size() <= 0)
		finish();

	//Update particles
	for (int i = 0; i < segments.size(); i++) {
		segments[i].update();

		//Remove them from the list if it's done
		if (segments[i].isDone()) {
			segments.erase(segments.begin() + i);
			i--;
		}
	}

	updateMesh();
}

void FX_Trail::draw() {
	FX::draw();

	mesh.draw();
}

////SEGMENTS

FX_Trail::Segment::Segment(glm::vec2 position, glm::vec2 direction, float size, float time) {
	this->position = position;
	this->direction = direction;
	this->size = size;
	timer.reset(time);
}

void FX_Trail::Segment::update() {
	timer.update();
}

array<float, 6> FX_Trail::Segment::getVertices() {
	if (isDone())
		return array<float, 6>();

	vec2 parallel = normalize(vec2(-direction.y, direction.x)) * getSize();

	array<float, 6> vertices;
	vertices[0] = position.x + parallel.x;
	vertices[1] = position.y + parallel.y;
	vertices[2] = 0.f;

	vertices[3] = position.x + -parallel.x;
	vertices[4] = position.y + -parallel.y;
	vertices[5] = 0.f;

	return vertices;
}

float FX_Trail::Segment::getSize() {
	if (timer.value < 0.2f)
		return size * timer.value / 0.2f;
	else
		return size * (1.f - (timer.value - 0.2f) / 0.8f);
}