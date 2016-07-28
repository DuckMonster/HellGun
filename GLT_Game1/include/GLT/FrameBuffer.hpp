#pragma once
#include <gl\glew.h>
#include "Texture.hpp"
#include "RenderBuffer.hpp"

namespace glt {
	class FrameBuffer {
	public:
		static size_t clientWidth;
		static size_t clientHeight;

		static void release() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); 
			glViewport(0, 0, clientWidth, clientHeight);
		}

		//---------

		FrameBuffer(size_t width, size_t height);
		~FrameBuffer();

		size_t getWidth() { return width; }
		size_t getHeight() { return height; }

		Texture* getTexture() { return &texture; }

		const GLuint getHandle() { return handle; }

		void bind();
		void display(Shader* shader = nullptr);

	private:
		//Static
		static Shader* display_shader;
		static VAO* display_VAO;
		static VBO* display_VBO_position,* display_VBO_uv;
		static void initDisplay();

		//----------

		GLuint handle;

		size_t width, height;

		//TEMPORARY STUFF
		Texture texture;
		RenderBuffer renderBuffer;
		//

		//----------

		void init();
	};
}