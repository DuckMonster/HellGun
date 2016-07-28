#pragma once
#include <string>
#include "MeshDrawer.hpp"
#include "TextMesh.hpp"
#include "Font.hpp"

namespace glt {
	class TextBox : public MeshDrawer {
	public:
		std::string text = "";
		Font* font = nullptr;

		//-----------

		TextBox();
		TextBox(std::string text);
		TextBox(Font* font);
		TextBox(std::string text, Font* font);

		TextMesh* getTextMesh() { return &textMesh; }

		void draw() override;

	private:
		TextMesh textMesh;
	};
}