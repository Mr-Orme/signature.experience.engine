#include <map>

#include "AssetLibrary.h"
#include "Texture.h"
#include "GraphicsDevice.h"
#include "SDL_mixer.h"
#include "ComponentsList.h"
#include "Object.h"


bool AssetLibrary::initialize(SoundDevice * sDevice, GraphicsDevice * gDevice)
{
	if (sDevice == nullptr || gDevice == nullptr)
	{
		printf("Bad sound or graphic's device");
		return false;
	}
	else
	{
		this->sDevice = sDevice;
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
	artLibrary[name] = std::make_unique<Texture>();
	if (!artLibrary[name]->load(gDevice->getRenderer(), path)) 
	{ 
		auto artIter = artLibrary.find(name);
		artLibrary.erase(artIter);
		return false; 
	}
	else
	{
		return true;
	}
}
//**************************************
//Based on your position in the game space and direction you are facing
//this method searches the library for the proper notice to display
//and returns that notice.
//if not found, it sets the text to nothing of the notice passed in and
//returns that notice.
AssetLibrary::Notice AssetLibrary::getNotice(std::string name)
//**************************************
{
	return noticeLibrary.find(name)->second;
}

bool AssetLibrary::setNotice(std::string name, Notice notice)
{
	noticeLibrary[name] = notice;	
	return true;
}

bool AssetLibrary::removeNotice(std::string name)
{
	auto toRemove = noticeLibrary.find(name);
	if (toRemove != noticeLibrary.end()) noticeLibrary.erase(toRemove);
	return false;
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

std::vector<Component*> AssetLibrary::getComponents(std::string name, Object * owner)
{
	//Vector of pointers to return
	//will be made into unique_ptrs in Object class
	std::vector<Component*> componentListPtrs;

	//finds the list of components associated with the name of the object passed.
	std::vector<AssetLibraryComponentList> componentList = componentLibrary.find(name)->second;

	//Iterate through the list of components
	//add the proper component to the list of components to return.
	for (auto comp : componentList)
	{
		switch (comp)
		{
		case AssetLibraryComponentList::BodyComp:
			componentListPtrs.push_back(new BodyComponent(owner));
			break;
		case AssetLibraryComponentList::HealthComp:
			componentListPtrs.push_back(new HealthComponent(owner));
			break;
		case AssetLibraryComponentList::RendererComp:
			componentListPtrs.push_back(new SpriteComponent(owner));
			break;
		case AssetLibraryComponentList::UserInputComp:
			componentListPtrs.push_back(new UserInputComponent(owner));
			break;
		case AssetLibraryComponentList::TextComp:
			componentListPtrs.push_back(new TextComponent(owner));
			break;
		default:
			break;
		}
	}
	//return the completed list
	return componentListPtrs;
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

Mix_Chunk * AssetLibrary::getSoundEffect(std::string name)
{
	auto soundIter = soundEffectLibrary.find(name);
	if (soundIter == soundEffectLibrary.end())
	{
		{printf("Sound Effect File not found!"); }
		return nullptr;
	}
	return soundIter->second;
}

Mix_Music * AssetLibrary::getMusic(std::string name)
{
	std::map<std::string,Mix_Music*>::iterator musicIter = musicLibrary.find(name);
	//make sure we found one.
	if (musicIter == musicLibrary.end())
	{
		{printf("Background File not found!"); }
		return nullptr;
	}
	else return musicIter->second;;
}

bool AssetLibrary::addSoundEffect(std::string name, std::string path)
{
	
	if (soundEffectLibrary[name] = Mix_LoadWAV(path.c_str())) return true;
	return false;
}

bool AssetLibrary::addBackgroundMusic(std::string name, std::string path)
{
	//Mix_Load
	if (musicLibrary[name] = Mix_LoadMUS(path.c_str())) return true;
	return false;
}
//**************************************
//Have not set this up yet, as there has bee no need for it.
bool AssetLibrary::removeSound(std::string name)
//**************************************
{
	return false;
}
