#include <map>

#include "AssetLibrary.h"
#include "Texture.h"
#include "GraphicsDevice.h"
#include "ComponentsList.h"
#include "Object.h"


bool AssetLibrary::initialize(GraphicsDevice * gDevice)
{
	if (!gDevice)
	{
		printf("Bad sound or graphic's device");
		return false;
	}
	else
	{
		this->gDevice = gDevice;
		return true;
	}

}

Texture * AssetLibrary::getArtAsset(std::string searchString)
{
	return artLibrary.find(searchString)->second.get();
}

bool AssetLibrary::setArtAsset(std::string name, std::string path)
{
	artLibrary[name] = std::make_unique<Texture>(gDevice, path);

	if (!artLibrary[name]->initialzied) 
	{ 
		auto artIter = artLibrary.find(name);
		artLibrary.erase(artIter);
		return false; 
	}

	return true;
}

ObjectFactory::ObjectFactoryPresets AssetLibrary::getObjectStats(std::string name)
{
	return (objectCreationLibrary.find(name)->second);
}

bool AssetLibrary::setObjectStats(std::string name, ObjectFactory::ObjectFactoryPresets stats)
{
	if (name == "")	return false;
	else
	{
		objectCreationLibrary[name] = stats;
		return true;
	}
}

PhysicsDevice::PhysicsStats AssetLibrary::getObjectPhysics(std::string name)
{
	return(physicsLibrary.find(name)->second);
}

bool AssetLibrary::setObjectPhysics(std::string name, PhysicsDevice::PhysicsStats physics)
{
	//this game never has friction.
	if (name == "") return false;
	else
	{
		physics.friction = 0.0f;
		physicsLibrary[name] = physics;
		return true;
	}
}

std::vector<AssetLibrary::AssetLibraryComponentList> AssetLibrary::getComponents(std::string name)
{
	return componentLibrary.find(name)->second;
}

bool AssetLibrary::setComponentList(std::string name, std::vector<AssetLibraryComponentList> componentList)
{
	if (name == "") return false;
	else
	{
		componentLibrary[name] = componentList;
		return true;
	}
}


