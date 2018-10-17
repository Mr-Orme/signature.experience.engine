#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Definitions.h"

class ObjectFactory;
class AssetLibrary;
class InputDevice;
class GraphicsDevice;
class PhysicsDevice;
class SoundDevice;

class ResourceManager
{
public:
	bool Initialize(EngineInt SCREEN_WIDTH, EngineInt SCREEN_HEIGHT, std::string assetPath);
	bool Shutdown();

	//Devices
	std::unique_ptr<GraphicsDevice> gDevice;
	std::unique_ptr<InputDevice> iDevice;
	std::unique_ptr<PhysicsDevice> pDevice;
	std::unique_ptr<SoundDevice> sDevice;

	//Library
	std::unique_ptr<AssetLibrary> assetLibrary;


	std::unique_ptr<ObjectFactory> factory;
	
	EngineInt FPS;
};

#endif