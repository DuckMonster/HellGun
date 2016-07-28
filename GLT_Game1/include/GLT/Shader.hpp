#pragma once
#include<string>
#include<GL\glew.h>

namespace glt {
	class Shader {
	public:
		static std::string ReadFile(const char* filename);

		Shader() {};
		Shader(const char*, const char*);
		~Shader() { dispose(); }

		void dispose() { glDeleteProgram(program); }

		void use();
		GLuint getUniform(const char*);
		GLuint getAttrib(const char*);

		operator GLuint() const { return program; }

	private:
		GLuint program;

		void init(const char*, const char*);
		GLuint createShader(GLenum, const char*);
	};
}