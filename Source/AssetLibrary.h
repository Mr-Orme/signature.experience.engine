#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include <map>
#include <string>
#include <vector>

#include "Definitions.h"
#include "ObjectFactory.h"
#include "PhysicsDevice.h"
#include "Texture.h"
#include "Sound.h"

class GraphicsDevice;
class Object;
class Component;

class BackgroundMusic;

class AssetLibrary
{
public:
	AssetLibrary() {};
	~AssetLibrary() {};
	bool initialize(ResourceManager* devices);

	Texture* getArtAsset(std::string name);
	bool addArtAsset(std::string name, std::string path);	
	
	bool addSoundEffect(std::string name, std::string path);
	bool addBackgroundMusic(std::string name, std::string path);

	SoundEffect* playSoundEffect(std::string name);
	BackgroundMusic* playBackgroundMusic(std::string name);

	bool hasSprites();
	bool hasSounds();

private:
	std::map<std::string, std::unique_ptr<Texture>> artLibrary;	
	std::map<std::string, std::unique_ptr<SoundEffect> > soundEffectLibrary;
	std::map<std::string, std::unique_ptr<BackgroundMusic> > musicLibrary;
	ResourceManager* devices{ nullptr };

};
#endif // !ASSETLIBRARIES_H
