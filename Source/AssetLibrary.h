#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include <map>
#include <string>

#include "Definitions.h"
#include "ObjectFactory.h"
#include "PhysicsDevice.h"


class GraphicsDevice;
class SoundDevice;
class Object;
class Component;
class Mix_Chunk;
class Mix_Music;

class AssetLibrary
{
public:
	bool initialize(SoundDevice* sDevice, GraphicsDevice* gDevice);

	Texture* getArtAsset(std::string name);
	bool setArtAsset(std::string name, std::string path);

	struct Notice
	{
		DIRECTION direction;
		std::string text;
	};
	//TODO: dealt with in event manager!
	Notice getNotice(Notice notice);
	bool setNotice(Notice notice);
	bool removeNotice(Notice notice);

	//TODO: switch from using name, to some sort of unique ID
	ObjectFactory::EngineObjectStats getObjectStats(std::string name);
	bool setObjectStats(std::string name, ObjectFactory::EngineObjectStats stats);

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
	std::map<std::string, ObjectFactory::EngineObjectStats> objectCreationLibrary;
	std::map<std::string, PhysicsDevice::PhysicsStats> physicsLibrary;
	std::map<std::string, std::vector<AssetLibraryComponentList>> componentLibrary;
	std::map<std::string, Mix_Chunk* > soundEffectLibrary;
	std::map<std::string, Mix_Music* > musicLibrary;
	
	SoundDevice* sDevice{ nullptr };
	GraphicsDevice* gDevice{ nullptr };

};
#endif // !ASSETLIBRARIES_H
