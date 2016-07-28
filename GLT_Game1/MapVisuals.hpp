#pragma once

#include <GLT\MeshDrawer.hpp>

class Map;

class MapVisuals {
public:
	MapVisuals(Map* map);

	void logic();
	void draw();

private:
	//Fog shader
	static glt::Shader* shader;
	static void compileShader();

	struct Cog {
	public:
		glt::MeshDrawer drawer;
		float speed;

		Cog(glm::vec3 position, glm::vec3 rotation, float size, float speed);

		void logic();
		void draw();
	};

	struct Chain {
	public:
		glt::MeshDrawer drawer;
		float speed;

		Chain(glm::vec3 position, glm::vec3 rotation, float size, float speed);

		void logic();
		void draw();
	};

	Map* map;

	//Meshes
	glt::MeshDrawer foregroundMesh;
	glt::MeshDrawer backgroundMesh;

	std::vector<Cog> cogList;
	std::vector<Chain> chainList;
};