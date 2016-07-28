#pragma once

#include <string>
#include <map>

#include "Font.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace glt {
	class TextMesh {
	public:
		//-------------

		TextMesh();
		TextMesh(Font* font);
		void setText(std::string text);
		void setFont(Font* font);
		Font* getFont() { return font; }

		void setAlign(int hAlign, int vAlign) { setHAlign(hAlign); setVAlign(vAlign); }
		void setHAlign(int hAlign);
		void setVAlign(int vAlign);
		int getHAlign();
		int getVAlign();

		float getSpacing() { return spacing; }
		void setSpacing(float spacing);

		Mesh* getMesh();

	private:
		Font* font = nullptr;
		std::string text = "";

		int hAlign = 0;
		int vAlign = 0;

		float spacing = 0.f;

		Mesh mesh;

		//------------

		void updateMesh();
	};
}