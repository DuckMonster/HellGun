#pragma once
#include <vector>
#include <map>
#include "Texture.hpp"

namespace glt {
	/// <summary>Parses a font from the program BMFont</summary>
	class Font {
	public:
		//Charinfo contains UV information
		struct CharInfo {
		public:
			unsigned int character;
			float x, y, width, height, aspect;

			CharInfo() {
				x = 0;
				y = 0;
				width = 0;
				height = 0;
				aspect = width / height;
			}
			CharInfo(unsigned int character, float x, float y, float width, float height) {
				this->character = character;
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;

				aspect = width / height;
			}
		};
		
		Font();
		Font(const char* fileName);
		std::vector<CharInfo> getString(std::string str);
		void loadFile(const char* file);

		Texture* getTexture() { return &texture; }

	private:
		std::map<unsigned int, CharInfo> charMap;

		//-----------

		Texture texture;
		std::vector<std::string> splitString(std::string input, char denomiator);
		std::string findValue(std::vector<std::string> params, std::string option);
	};
}