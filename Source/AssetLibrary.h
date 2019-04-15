#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include <string>
#include <map>
#include "Definitions.h"


class ResourceManager;
class Texture;
class SoundEffect;
class BackgroundMusic;
class GraphicsDevice;
class Object;
class Component;

class AssetLibrary
{
public:
	AssetLibrary(ResourceManager* devices);
	AssetLibrary() = delete;
	~AssetLibrary();
	
	//Todo::Will convert all string values to pass by reference. const, if possible
	std::shared_ptr<Texture> getArtAsset(std::string name);
	bool addArtAsset(std::string name, std::string path);	
	
	bool addSoundEffect(std::string name, std::string path);
	bool addBackgroundMusic(std::string name, std::string path);

	SoundEffect* playSoundEffect(std::string &name);
	BackgroundMusic* playBackgroundMusic(std::string &name);

	bool hasSprites();
	bool hasSounds();

private:
	//Will pass string values to pass by reference
	std::map<std::string, std::shared_ptr<Texture>> artLibrary;	
	std::map<std::string, std::unique_ptr<SoundEffect> > soundEffectLibrary;
	std::map<std::string, std::unique_ptr<BackgroundMusic> > musicLibrary;
	ResourceManager* devices{ nullptr };

};
#endif // !ASSETLIBRARIES_H
