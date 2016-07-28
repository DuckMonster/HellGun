#pragma once
#include <GL\glew.h>

class Stencil {
public:
	static void func(GLenum func, GLuint value);
	static void op(GLenum sFail, GLenum dFail, GLenum pass);

	static void start();
	static void end();
	static void clear();
	
	static void setColorDraw(bool enabled);
};