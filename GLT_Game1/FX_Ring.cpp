#include "FX_Ring.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
using namespace std;

#include <GLT\Stencil.hpp>

#include "MathUtil.hpp"

using namespace glt;

FX_Ring::FX_Ring(glm::vec2 position, float size, float time) : mesh(new Mesh()), FX() {
	//Create ring mesh
	vector<float> vertices;

	int resolution = size * 20.f;
	float angle = (M_PI * 2.f) / (float)resolution;

	for (int i = 0; i < resolution; i++) {
		//Add base
		vertices.push_back(0.f);
		vertices.push_back(0.f);
		vertices.push_back(0.f);

		//Add first
		vertices.push_back(cos(angle * i));
		vertices.push_back(sin(angle * i));
		vertices.push_back(0.f);

		//Second
		vertices.push_back(cos(angle * (i + 1)));
		vertices.push_back(sin(angle * (i + 1)));
		vertices.push_back(0.f);
	}

	mesh.getMesh()->setVertices(&vertices[0], vertices.size() * sizeof(float));
	// -----

	mesh.position = glm::vec3(position, 0.f);
	this->size = size;
	timer.reset(time);
}
FX_Ring::~FX_Ring() {
	FX::~FX();
	delete mesh.getMesh();
}

void FX_Ring::logic() {
	FX::logic();

	timer.update();
	if (timer.isDone())
		finish();
}

void FX_Ring::draw() {
	FX::draw();

	//sprite.scale = glm::vec2(linToExp(timer.value) * size);
	//sprite.draw();

	Stencil::start();
	{
		Stencil::clear();

		Stencil::op(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		Stencil::func(GL_ALWAYS, 1);

		Stencil::setColorDraw(false);

		mesh.scale = glm::vec3(linToExp(timer.value) * size * 0.5f + timer.value * size * 0.5f);
		mesh.draw();
		
		Stencil::setColorDraw(true);
		Stencil::func(GL_NOTEQUAL, 1);

		mesh.scale = glm::vec3(linToExp(timer.value) * size);
		mesh.draw();
	}
	Stencil::end();
}