#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <memory>
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
	bool initialize(std::string assetPath, GraphicsDevice* gDevice);
	bool shutdown();

	//Devices
	GraphicsDevice* gDevice{ nullptr };
	std::unique_ptr<InputDevice> iDevice{ nullptr };
	std::unique_ptr<PhysicsDevice> pDevice{ nullptr };
	std::unique_ptr<SoundDevice> sDevice{ nullptr };

	//Library
	std::unique_ptr<AssetLibrary> assetLibrary{ nullptr };
	std::unique_ptr<ObjectFactory> factory{ nullptr };

	std::vector<std::unique_ptr<Object>> objects;
	
	EngineInt FPS{ 0 };

private:
	std::vector<std::unique_ptr<Object>> newObjects;
};

#endif