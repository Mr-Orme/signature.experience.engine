#ifndef SOUND_H
#define SOUND_H

#include <string>
#include "SDL_mixer.h"

class SoundDevice;

class SoundEffect
{
public:
	SoundEffect() = delete;
	~SoundEffect();
	//passing string value by reference
	SoundEffect(std::string &path, SoundDevice* sDevice);
	void playSound(int numLoops=1);
	Mix_Chunk * effect{ nullptr };
private:
	SoundDevice* sDevice;
};

class BackgroundMusic
{
public:
	BackgroundMusic() = delete;
	~BackgroundMusic();
	//passing string value by reference
	BackgroundMusic(std::string &path, SoundDevice* sDevice);
	void setAsBackground();
	Mix_Music * background{ nullptr };
private:
	SoundDevice * sDevice;
};
#endif // !SOUND_H


