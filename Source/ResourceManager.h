#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Definitions.h"
#include "GraphicsDevice.h"
#include "InputDevice.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "AssetLibrary.h"
#include "ObjectFactory.h"

class ResourceManager
{
public:
	//TODO:: move initialize to constructor and shutdown to destructor
	ResourceManager(std::string assetPath);
	~ResourceManager();
	//deleted initialize and shutdown while moving functionality into constructor and destructor.
	//I feel like since shutdown was deleted if we happen to reference shutdown within another file we may have
	//to change up some things.

	void update();
	void draw();
	//TODO:: use this to test out killing objects and destroying joints.
	bool killObject(Object* butAScratch);
	//Devices
	std::unique_ptr<GraphicsDevice> gDevice{ nullptr };
	std::unique_ptr<InputDevice> iDevice{ nullptr };
	std::unique_ptr<PhysicsDevice> pDevice{ nullptr };
	std::unique_ptr<SoundDevice> sDevice{ nullptr };

	//Library
	std::unique_ptr<AssetLibrary> assetLibrary{ nullptr };
	
	//factory
	std::unique_ptr<ObjectFactory> factory{ nullptr };

	std::vector<std::unique_ptr<Object>> objects;
	
	EngineDefs::Int FPS{ 0 };

private:
	std::vector<std::unique_ptr<Object>> newObjects;
};

#endif