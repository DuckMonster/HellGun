#pragma once
#include <map>
#include <vector>
#include <string>
#include <GLT\Mesh.hpp>
#include <GLT\Texture.hpp>
#include <GLT\Font.hpp>
#include <SFML\Audio.hpp>

class AssetBank {
public:
	static std::string assetRoot;
	static std::string textureRoot;
	static std::string meshRoot;
	static std::string fontRoot;
	static std::string soundRoot;

	static glt::Mesh* loadMesh(std::string name, size_t index = 0);
	static glt::Texture* loadTexture(std::string name);
	static glt::Font* loadFont(std::string name);
	static sf::Sound* loadSound(std::string name);

private:
	static std::map<std::string, std::vector<glt::Mesh*>> meshBank;
	static std::map<std::string, glt::Texture*> textureBank;
	static std::map<std::string, glt::Font*> fontBank;
	static std::map<std::string, sf::SoundBuffer*> soundBank;
};