#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include <map>
#include <string>
#include <vector>

#include "Definitions.h"
#include "ObjectFactory.h"
#include "PhysicsDevice.h"
#include "Texture.h"

class GraphicsDevice;
class Object;
class Component;

class AssetLibrary
{
public:
	AssetLibrary() {};
	~AssetLibrary() {};
	bool initialize(GraphicsDevice* gDevice);

	Texture* getArtAsset(std::string name);
	bool setArtAsset(std::string name, std::string path);

	//TODO: switch from using name, to some sort of unique ID
	ObjectFactory::ObjectFactoryPresets getObjectStats(std::string name);
	bool setObjectStats(std::string name, ObjectFactory::ObjectFactoryPresets stats);

	PhysicsDevice::PhysicsStats getObjectPhysics(std::string name);
	bool setObjectPhysics(std::string name, PhysicsDevice::PhysicsStats physics);

	enum class AssetLibraryComponentList
	{
		BodyComp,
		HealthComp,
		SpriteComp,
		UserInputComp
	};
	std::vector<AssetLibraryComponentList> getComponents(std::string name);
	bool setComponentList(std::string name, std::vector<AssetLibraryComponentList> componentList);

private:
	std::map<std::string, std::unique_ptr<Texture>> artLibrary;
	std::map<std::string, ObjectFactory::ObjectFactoryPresets> objectCreationLibrary;
	std::map<std::string, PhysicsDevice::PhysicsStats> physicsLibrary;
	std::map<std::string, std::vector<AssetLibraryComponentList>> componentLibrary;
	
	
	GraphicsDevice* gDevice{ nullptr };

};
#endif // !ASSETLIBRARIES_H
