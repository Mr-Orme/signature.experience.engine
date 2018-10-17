#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include <string>

#include "SDL_Mixer.h"

#include "Definitions.h"



class ResourceManager;

class SoundDevice{
public:
	~SoundDevice();
	bool initialize(ResourceManager* devices);
	void Shutdown();

	bool PlaySound(std::string sound, int numLoops);
	bool PlaySound(std::string sound, int numLoops, int channel);
	
	void setBackground(std::string background);
	
private:
	ResourceManager* devices;
};


#endif