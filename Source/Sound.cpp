#include "SDL_Mixer.h"

#include "Sound.h"
#include "SoundDevice.h"

SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(effect);
}

SoundEffect::SoundEffect(std::string &path, SoundDevice* sDevice)
{
	this->sDevice = sDevice;
	effect = Mix_LoadWAV(path.c_str());
}

void SoundEffect::playSound(int numLoops)
{
	sDevice->playSound(this, numLoops);
}

BackgroundMusic::~BackgroundMusic()
{
	Mix_FreeMusic(background);
}

BackgroundMusic::BackgroundMusic(std::string &path, SoundDevice* sDevice)
{
	this->sDevice = sDevice;
	background = Mix_LoadMUS(path.c_str());
}

void BackgroundMusic::setAsBackground()
{
	sDevice->setAsBackground(this);
}
