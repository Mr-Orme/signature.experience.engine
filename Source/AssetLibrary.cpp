#include <map>

#include "AssetLibrary.h"
#include "Texture.h"
#include "GraphicsDevice.h"

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
AssetLibrary::Notice AssetLibrary::getNotice(Notice square)
//**************************************
{
	for (auto notice : noticeLibrary)
	{
		if (notice.position == square.position && notice.direction == square.direction)
		{
			return notice;
		}
	}
	square.text = "";
	return square;
}

bool AssetLibrary::setNotice(Notice notice)
{
	if (notice.text == "")
	{
		return false;
	}
	else
	{
		noticeLibrary.push_back(notice);
	}
}

bool AssetLibrary::removeNotice(Notice notice)
{
	std::vector<Notice>::iterator noticeIter;
	//iterate through all notices
	for (noticeIter = noticeLibrary.begin(); noticeIter != noticeLibrary.end(); noticeIter++)
	{
		//if position and direction match, delete the notice from the library.
		if (noticeIter->position == notice.position && noticeIter->direction == notice.direction)
		{
			noticeLibrary.erase(noticeIter);
			noticeIter--;
			return true;
		}
	}
	//if we did not find a notice, we return false.
	return false;
}

ObjectFactory::EngineObjectStats AssetLibrary::getObjectStats(std::string name)
{
	return (objectCreationLibrary.find(name)->second);
}

bool AssetLibrary::setObjectStats(std::string name, ObjectFactory::EngineObjectStats stats)
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
			componentListPtrs.push_back(new RendererComponent(owner));
			break;
		case AssetLibraryComponentList::UserInputComp:
			componentListPtrs.push_back(new UserInputComponent(owner));
			break;
		case AssetLibraryComponentList::BackpackComp:
			componentListPtrs.push_back(new BackpackComponent(owner));
			break;
		case AssetLibraryComponentList::InventoryComp:
			componentListPtrs.push_back(new InventoryComponent(owner));
			break;
		case AssetLibraryComponentList::GhostComp:
			componentListPtrs.push_back(new GhostComponent(owner));
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
