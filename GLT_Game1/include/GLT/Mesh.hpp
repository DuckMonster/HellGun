#pragma once
#include<glm\vec3.hpp>
#include<glm\matrix.hpp>
#include<vector>

#include "VAO.hpp"
#include "VBO.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace glt {
	class Mesh {
	public:
		GLuint drawMode = GL_TRIANGLES;

		//-----------

		Mesh();

		void setVertices(const float*, int);
		void setUVS(const float*, int);
		void setColors(const float*, int);

		VAO* getVAO() { return &vao; }
		VBO* getVertexVBO() { return &vbo_vertices; }
		VBO* getColorVBO() { return &vbo_colors; }
		VBO* getUvVBO() { return &vbo_uvs; }

		void setDataUsage(GLenum usage);

		void draw();

	private:
		VAO vao;
		VBO vbo_vertices, vbo_colors, vbo_uvs;
		std::size_t vertex_count = 0;

		//-----------

		void init();
		void fillVertices(size_t);
	};
}