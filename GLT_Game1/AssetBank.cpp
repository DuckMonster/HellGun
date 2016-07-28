#include "AssetBank.hpp"

#include <GLT\MeshLoader.hpp>

using namespace glt;
using namespace std;
using namespace sf;

map<string, vector<Mesh*>> AssetBank::meshBank;
map<string, Texture*> AssetBank::textureBank;
map<string, Font*> AssetBank::fontBank;
map<string, SoundBuffer*> AssetBank::soundBank;

string AssetBank::assetRoot = "./Assets/";
string AssetBank::textureRoot = "Texture/";
string AssetBank::meshRoot = "Mesh/";
string AssetBank::fontRoot = "Font/";
string AssetBank::soundRoot = "Sound/";

Mesh* AssetBank::loadMesh(string name, size_t index) {
	if (meshBank.find(name) == meshBank.end())
		meshBank[name] = MeshLoader::loadFBX((assetRoot + meshRoot + name + ".fbx").c_str());

	return meshBank[name][index];
}

Texture* AssetBank::loadTexture(string name) {
	if (textureBank.find(name) == textureBank.end())
		textureBank[name] = new Texture((assetRoot + textureRoot + name + ".png").c_str());

	return textureBank[name];
}

Font* AssetBank::loadFont(string name) {
	if (fontBank.find(name) == fontBank.end())
		fontBank[name] = new Font((assetRoot + fontRoot + name + ".fnt").c_str());

	return fontBank[name];
}

Sound* AssetBank::loadSound(string name) {
	if (soundBank.find(name) == soundBank.end()) {
		SoundBuffer* buffer = new SoundBuffer();
		buffer->loadFromFile(assetRoot + soundRoot + name + ".wav");

		soundBank[name] = buffer;
	}

	return new Sound(*soundBank[name]);
}