#pragma once
#include <GL\glew.h>
#include <iostream>
#include "VBO.hpp"

namespace glt {
	class VAO {
	public:
		VAO() { init(); }
		~VAO() { dispose(); }

		void dispose() { glDeleteVertexArrays(1, &handle); }

		void bind() { glBindVertexArray(handle); }

		void bindBufferToAttr(VBO* vbo, GLuint attribute);
		void bindBufferToAttr(GLuint vbo, GLuint size, GLuint attribute);

		operator GLuint() const { return handle; }

	private:
		GLuint handle;

		void init() { glGenVertexArrays(1, &handle); }
	};
}