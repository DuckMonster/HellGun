#pragma once
#include <vector>
#include <iostream>
#include <GL\glew.h>

namespace glt {
	class VBO {
	public:
		GLenum dataUsage = GL_STATIC_DRAW;

		VBO() { init(3); }
		VBO(size_t size) { init(size); }
		~VBO() { dispose(); }

		void init(size_t);

		void dispose() { glDeleteBuffers(1, &handle); }

		void bind();
		void setData(const std::vector<float>);
		void setData(const float*, size_t);

		const std::vector<float> getData();

		void setDataSize(size_t size) { this->dataSize = size; }
		size_t getDataSize() { return dataSize; }

		void setVertexCount(size_t, float);
		size_t getVertexCount();

		GLuint getHandle() const { return handle; }

		operator GLuint() const { return getHandle(); }

	private:
		GLuint handle = -1;
		size_t dataSize = 3;

		std::vector<float> data;
	};
}