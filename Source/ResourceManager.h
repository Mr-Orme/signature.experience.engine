#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Definitions.h"

class ObjectFactory;
class AssetLibrary;
class SoundDevice;
class PhysicsDevice;
class InputDevice;
class GraphicsDevice;
class EventHandler;
class Object;
class ResourceManager
{
public:
	
	ResourceManager() = delete;
	ResourceManager(std::string assetPath);
	~ResourceManager();
	
	void update();
	void draw();
	//TODO::Josh::use this to test out killing objects and destroying joints.
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
	std::unique_ptr<EventHandler> eventHandler{ nullptr };
	
	EngineDefs::Int FPS{ 0 };

private:
	std::vector<std::unique_ptr<Object>> newObjects;
};

#endif