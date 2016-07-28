#pragma once
#include<GL\glew.h>
#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

namespace glt {
	class FrameBuffer;

	class RenderBuffer {
	public:
		RenderBuffer();

		void bind();
		void bindToFramebuffer(FrameBuffer* buffer);

	private:
		GLuint handle;

		size_t width, height;

		//---------

		void init();
	};
}