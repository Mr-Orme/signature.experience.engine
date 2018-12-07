#include <map>

#include "AssetLibrary.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "ComponentsList.h"
#include "Object.h"


bool AssetLibrary::initialize(ResourceManager* devices)
{
	this->devices = devices;
	return true;
}

Texture * AssetLibrary::getArtAsset(std::string &searchString)
{
	return artLibrary.find(searchString)->second.get();
}

bool AssetLibrary::addArtAsset(std::string &name, std::string &path)
{
	artLibrary[name] = std::make_unique<Texture>(devices->gDevice.get(), path);

	if (!artLibrary[name]->initialzied) 
	{ 
		auto artIter = artLibrary.find(name);
		artLibrary.erase(artIter);
		return false; 
	}

	return true;
}

bool AssetLibrary::addSoundEffect(std::string &name, std::string &path)
{
	soundEffectLibrary[name] = std::make_unique<SoundEffect>(path, devices->sDevice.get());
	return true;
	
}

bool AssetLibrary::addBackgroundMusic(std::string &name, std::string &path)
{
	musicLibrary[name] = std::make_unique<BackgroundMusic>(path, devices->sDevice.get());
	return true;
}

SoundEffect * AssetLibrary::playSoundEffect(std::string &name)
{
	if (auto sound = soundEffectLibrary.find(name); sound != soundEffectLibrary.end())
	{
		return sound->second.get();
	}
	else
	{
		return nullptr;
	}

}

BackgroundMusic * AssetLibrary::playBackgroundMusic(std::string &name)
{
	if (auto sound = musicLibrary.find(name); sound != musicLibrary.end())
	{
		return sound->second.get();
	}
	else
	{
		return nullptr;
	}
}

bool AssetLibrary::hasSprites()
{
	if (artLibrary.empty())
	{
		return false;
	}
	return true;
}

bool AssetLibrary::hasSounds()
{
	if (soundEffectLibrary.empty() && musicLibrary.empty())
	{
		return false;
	}
	return true;
}




