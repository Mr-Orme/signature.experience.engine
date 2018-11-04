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




