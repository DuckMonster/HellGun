#pragma once
#include <GL\glew.h>

namespace glt {
	void gltInit() {
		glewExperimental = true;
		glewInit();
	}
}