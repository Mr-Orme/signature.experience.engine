#include "SoundDevice.h"
#include "SDL_mixer.h"


SoundDevice::~SoundDevice()
{
	Mix_CloseAudio();
	Mix_Quit();
}

//**************************************
//set's up initial setting for sound device
SoundDevice::SoundDevice()
//**************************************
{
	//allows for OGG support
	{
		auto flags = MIX_INIT_OGG;
		auto initted = Mix_Init(flags);

		if (initted && flags != flags)
		{
			printf("Mix_Init: Failed to init required ogg and mod support!\n");
			printf("Mix_Init: %s\n", Mix_GetError());
			initialized = false;
		}
	}

	//Load the Mixer subsystem
	if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096 ) <0)
	{
		printf( "SDL Mixer could not initialize! SDL_Error: %s\n", Mix_GetError() );
		initialized = false;
	}

	//Allocate sufficient channels
    Mix_AllocateChannels(100);
	initialized = true;
}

//**************************************
//plays a sound for a number of loops, must be in the sound library.
bool SoundDevice::PlaySound(std::string sound, int numLoops)
//**************************************
{
	int channelID = -1; //Select first available channel
	PlaySound(sound, numLoops, channelID);
	return true;
}
//**************************************
//same as above, but here we can specify a channel to play the sound on.
bool SoundDevice::PlaySound(std::string sound, int numLoops, int channel)
//**************************************
{
	Mix_PlayChannel(channel, getSoundEffect(sound), numLoops);
	return true;
}
//**************************************
//set's the background music to play.
void SoundDevice::setBackground(std::string background)
//**************************************
{
	if(Mix_PlayMusic(getMusic(background), -1) == -1)
	{printf("Mix_PlayMusic: %s\n", Mix_GetError());}
}
Mix_Chunk * SoundDevice::getSoundEffect(std::string name)
{
	auto soundIter = soundEffectLibrary.find(name);
	if (soundIter == soundEffectLibrary.end())
	{
		{printf("Sound Effect File not found!"); }
		return nullptr;
	}
	return soundIter->second;
}

Mix_Music * SoundDevice::getMusic(std::string name)
{
	std::map<std::string, Mix_Music*>::iterator musicIter = musicLibrary.find(name);
	//make sure we found one.
	if (musicIter == musicLibrary.end())
	{
		{printf("Background File not found!"); }
		return nullptr;
	}
	else return musicIter->second;;
}

bool SoundDevice::addSoundEffect(std::string name, std::string path)
{

	if (soundEffectLibrary[name] = Mix_LoadWAV(path.c_str())) return true;
	return false;
}

bool SoundDevice::addBackgroundMusic(std::string name, std::string path)
{
	//Mix_Load
	if (musicLibrary[name] = Mix_LoadMUS(path.c_str())) return true;
	return false;
}
//**************************************
//Have not set this up yet, as there has bee no need for it.
bool SoundDevice::removeSound(std::string name)
//**************************************
{
	return false;
}