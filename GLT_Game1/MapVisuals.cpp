#include "MapVisuals.hpp"

#include <glm\gtc\type_ptr.hpp>
#include <GLT\Stencil.hpp>
#include <GLT\Time.hpp>

#include "GameState.hpp"
#include "AssetBank.hpp"
#include "ColorConfig.hpp"
#include "Map.hpp"

using namespace glt;

const char* MAP_SRC_VERTEX =
"#version 450\n"
"in vec3 v_position;"

"uniform mat4 u_camera;"
"uniform mat4 u_model;"
"out float f_depth;"

"void main() {"
"	gl_Position = u_camera * u_model * vec4(v_position, 1.0);"
"	f_depth = gl_Position.z;"
"}";

const char* MAP_SRC_FRAGMENT =
"#version 450\n"
"in float f_depth;"

"uniform vec4 u_blendColor;"
"uniform vec4 u_fogColor;"

"out vec4 outColor;"

"void main() {"
"	outColor = mix(u_blendColor, u_fogColor, -f_depth / -150.0);"
"}";

Shader* MapVisuals::shader = nullptr;
void MapVisuals::compileShader() {
	if (shader != nullptr)
		return;

	shader = new Shader(MAP_SRC_VERTEX, MAP_SRC_FRAGMENT);
}

MapVisuals::MapVisuals(Map* map) {
	compileShader();

	this->map = map;

	foregroundMesh.setMesh(AssetBank::loadMesh("plane"));

	backgroundMesh.setShader(shader);
	backgroundMesh.setMesh(AssetBank::loadMesh("map", 1));

	cogList.push_back(Cog(vec3(0.f, 0.f, -30.f), vec3(0.f), 40.f, 30.f));
	cogList.push_back(Cog(vec3(-20.f, 0.f, -30.f), vec3(0.f, 50.f, 0.f), 20.f, 180.f));
	cogList.push_back(Cog(vec3(10.f, 10.f, -30.f), vec3(60.f, -40.f, 0.f), 50.f, 45.f));

	//chainList.push_back(Chain(vec3(-10.f, 30.f, -5.f), vec3(0.f), 50.f, 1.5f));
}

void MapVisuals::logic() {
	for (int i = 0; i < cogList.size(); i++)
		cogList[i].logic();

	for (int i = 0; i < chainList.size(); i++)
		chainList[i].logic();
}

void MapVisuals::draw() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Draw foreground
	Stencil::start();
	Stencil::clear();

	Stencil::func(GL_NEVER, 1);
	Stencil::op(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	Stencil::setColorDraw(false);

	foregroundMesh.scale = vec3(map->getSize(), 1.f);
	foregroundMesh.draw();

	Stencil::func(GL_NOTEQUAL, 1);
	Stencil::setColorDraw(true);

	foregroundMesh.color = ColorConfig::getForeground();
	foregroundMesh.scale = vec3(5000.f);
	foregroundMesh.draw();

	Stencil::end();

	shader->use();
	glUniform4fv(shader->getUniform("u_fogColor"), 1, glm::value_ptr(ColorConfig::getFog()));
	backgroundMesh.color = ColorConfig::getBackground();
	backgroundMesh.draw();

	for (int i = 0; i < cogList.size(); i++)
		cogList[i].draw();

	for (int i = 0; i < chainList.size(); i++)
		chainList[i].draw();

	glDisable(GL_DEPTH_TEST);
}

//Cog wheel
MapVisuals::Cog::Cog(vec3 position, vec3 rotation, float size, float speed) {
	this->speed = speed;

	drawer.setShader(MapVisuals::shader);
	drawer.setMesh(AssetBank::loadMesh("mapProps", 2));
	drawer.position = position;
	drawer.rotation = rotation;
	drawer.scale = vec3(size);
}

void MapVisuals::Cog::logic() {
	drawer.rotation.z += speed * GameState::deltaTime;
}

void MapVisuals::Cog::draw() {
	drawer.color = ColorConfig::getBackground();
	drawer.draw();
}

//Chain
MapVisuals::Chain::Chain(vec3 position, vec3 rotation, float size, float speed) {
	this->speed = speed;

	drawer.setShader(MapVisuals::shader);
	drawer.setMesh(AssetBank::loadMesh("mapProps", 1));
	drawer.position = position;
	drawer.rotation = rotation;
	drawer.scale = vec3(size);
}

void MapVisuals::Chain::logic() {
	vec3 up(0.f, 1.f, 0.f);
	up = vec3(drawer.modelMatrix * vec4(up, 1.f));

	drawer.rotation += up * speed;
}

void MapVisuals::Chain::draw() {
	drawer.color = ColorConfig::getBackground();
	drawer.draw();
}