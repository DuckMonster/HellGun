#pragma once
#include<SOIL\SOIL.h>
#include<GL\glew.h>

namespace glt {
	class FrameBuffer;

	class Texture {
	public:
		Texture();
		Texture(const char* source);
		~Texture();

		void dispose();

		int getWidth() { return width; }
		int getHeight() { return height; }

		void bind();
		void bindToFramebuffer(FrameBuffer* framebuffer);
		void loadFile(const char* source);

		operator GLuint() { return handle; }

	private:
		int width, height;
		GLuint handle;

		//-----------

		void init();
	};
}