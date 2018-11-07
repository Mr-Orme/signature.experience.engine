#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include <string>
#include <map>
#include "Definitions.h"
#include "SDL_mixer.h"

class ResourceManager;
class AssetLibrary;
class SoundEffect;
class BackgroundMusic;

class SoundDevice {
public:
	~SoundDevice();
	SoundDevice() = delete;
	SoundDevice(AssetLibrary* aLibrary);


	bool playSound(std::string sound, int numLoops=1, int channel=-1);
	bool playSound(SoundEffect* sound, int numLoops=1, int channel=-1);

	void setAsBackground(std::string background);
	void setAsBackground(BackgroundMusic* background);
	
	bool removeSound(std::string name);
	bool getInitialized() { return initialized; }
private:
	bool initialized{ false };
	AssetLibrary* aLibrary{ nullptr };
};


#endif