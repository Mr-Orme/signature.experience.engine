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
	
	enum class AssetLibraryComponentList
	{
		BodyComp,
		HealthComp,
		SpriteComp,
		UserInputComp
	};
	

private:
	std::map<std::string, std::unique_ptr<Texture>> artLibrary;	
	GraphicsDevice* gDevice{ nullptr };

};
#endif // !ASSETLIBRARIES_H
