#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include <map>
#include <string>
#include <vector>

#include "Definitions.h"
#include "ObjectFactory.h"
#include "PhysicsDevice.h"
#include "SDL_mixer.h"
#include "Texture.h"


class GraphicsDevice;
class SoundDevice;
class Object;
class Component;


class AssetLibrary
{
public:
	AssetLibrary() {};
	~AssetLibrary() {};
	bool initialize(SoundDevice* sDevice, GraphicsDevice* gDevice);

	Texture* getArtAsset(std::string name);
	bool setArtAsset(std::string name, std::string path);

	struct Notice
	{
		Direction direction;
		std::string text;
	};
	//TODO: dealt with in event manager!
	Notice getNotice(std::string name);
	bool setNotice(std::string name, Notice notice);
	bool removeNotice(std::string name);

	//TODO: switch from using name, to some sort of unique ID
	ObjectFactory::ObjectFactoryPresets getObjectStats(std::string name);
	bool setObjectStats(std::string name, ObjectFactory::ObjectFactoryPresets stats);

	PhysicsDevice::PhysicsStats getObjectPhysics(std::string name);
	bool setObjectPhysics(std::string name, PhysicsDevice::PhysicsStats physics);

	enum class AssetLibraryComponentList
	{
		BodyComp,
		HealthComp,
		RendererComp,
		UserInputComp,
		BackpackComp,
		InventoryComp,
		GhostComp,
	};
	std::vector<Component*> getComponents(std::string name, Object* owner);
	bool setComponentList(std::string name, std::vector<AssetLibraryComponentList> componentList);

	//TODO: need to get the SDL_mixer references out of  here!
	Mix_Chunk* getSoundEffect(std::string name);
	Mix_Music* getMusic(std::string name);
	bool addSoundEffect(std::string name, std::string path);
	bool addBackgroundMusic(std::string name, std::string path);
	bool removeSound(std::string name);

	


private:
	std::map<std::string, std::unique_ptr<Texture>> artLibrary;
	std::map<std::string, Notice> noticeLibrary;
	std::map<std::string, ObjectFactory::ObjectFactoryPresets> objectCreationLibrary;
	std::map<std::string, PhysicsDevice::PhysicsStats> physicsLibrary;
	std::map<std::string, std::vector<AssetLibraryComponentList>> componentLibrary;
	std::map<std::string, Mix_Chunk* > soundEffectLibrary;
	std::map<std::string, Mix_Music* > musicLibrary;
	
	SoundDevice* sDevice{ nullptr };
	GraphicsDevice* gDevice{ nullptr };

};
#endif // !ASSETLIBRARIES_H
