#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include <string>
#include <map>
#include "Definitions.h"
#include "SDL_mixer.h"

class ResourceManager;

class SoundDevice {
public:
	~SoundDevice();
	SoundDevice();


	bool PlaySound(std::string sound, int numLoops);
	bool PlaySound(std::string sound, int numLoops, int channel);

	void setBackground(std::string background);
	//TODO:: Make Sound class similar to Texture class, but for sounds!
	Mix_Chunk* getSoundEffect(std::string name);
	Mix_Music* getMusic(std::string name);
	bool addSoundEffect(std::string name, std::string path);
	bool addBackgroundMusic(std::string name, std::string path);
	bool removeSound(std::string name);
	bool getInitialized() { return initialized; }
private:

	std::map<std::string, Mix_Chunk* > soundEffectLibrary;
	std::map<std::string, Mix_Music* > musicLibrary;
	bool initialized{ false };
};


#endif