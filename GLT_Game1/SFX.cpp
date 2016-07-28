#include "SFX.hpp"

#include "AssetBank.hpp"
#include "SoundConfig.hpp"

using namespace std;

SFX::SFX(string filename) {
	load(filename);
}

SFX::~SFX() {
	if (sound != nullptr)
		delete sound;
}

void SFX::load(string filename) {
	if (sound != nullptr)
		delete sound;

	sound = AssetBank::loadSound(filename);
}

void SFX::play() {
	if (sound == nullptr)
		return;

	sound->setVolume(SoundConfig::effectVolume * volume);
	sound->play();
}