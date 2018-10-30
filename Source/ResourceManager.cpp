
#include <iostream>
#include "ResourceManager.h"

#include "InputDevice.h"
#include "GraphicsDevice.h"
#include "View.h"
#include "Texture.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"

#include "AssetLibrary.h"
#include "ObjectFactory.h"

#include "Definitions.h"
#include "tinyxml2.h"

bool createThisDevice(tinyxml2::XMLElement* createMe)
{
	bool create;
	createMe->QueryBoolAttribute("create", &create);
	return create;
}
//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
bool ResourceManager::initialize(std::string assetPath, GraphicsDevice* gDevice)
{
	tinyxml2::XMLDocument levelConfig;
	if (!levelConfig.LoadFile(assetPath.c_str())) { return false; };
	
	tinyxml2::XMLElement* asset = levelConfig.FirstChildElement();
	FPS = std::stoi(asset->GetText());
	
	asset = asset->NextSiblingElement();
	tinyxml2::XMLElement* deviceCreationElement = asset->FirstChildElement();

	this->gDevice = gDevice;

	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_unique<ObjectFactory>();

	//========================================
	//Construct Input Device
	//========================================
	
	if (createThisDevice(deviceCreationElement))
	{
		//TODO:: load inputs from file.
		iDevice = std::make_unique<InputDevice>();
		if (!iDevice->initialize())
		{
			printf("Input Device could not initialize!");
			exit(1);
		}
	}
	
	//========================================
	//Construct Physics Device
	//========================================
	deviceCreationElement = deviceCreationElement->NextSiblingElement();
	if (createThisDevice(deviceCreationElement))
	{
		Position gravity{ 0, 0 };
		deviceCreationElement->QueryFloatAttribute("gravityX", &gravity.x);
		deviceCreationElement->QueryFloatAttribute("gravityY", &gravity.y);
		pDevice = std::make_unique<PhysicsDevice>(gravity);

		if (!pDevice->initialize())
		{
			printf("Physics Device could not intialize!");
			exit(1);
		}
	}
	

	//========================================
	//Construct Asset Library
	//========================================
	deviceCreationElement = deviceCreationElement->NextSiblingElement();
	if (createThisDevice(deviceCreationElement))
	{
		assetLibrary = std::make_unique<AssetLibrary>();
		//TODO::needs to be an xml file just likes physics.
		if (!assetLibrary->initialize(sDevice.get(), gDevice)) { exit(1); }

		asset = asset->NextSiblingElement();

		//add each asset into each library.
		do
		{
			//get the name of the asset
			std::string aName = asset->Attribute("name");

			//where we will store the components.
			std::vector<AssetLibrary::AssetLibraryComponentList> componentList;

			//move to the components of the xml
			tinyxml2::XMLElement* compElement = asset->FirstChildElement();

			//Add each component to the vector
			while (compElement)
			{
				//get the component's name
				std::string currentComponent = compElement->Attribute("name");
				//find the component we just grabbed
				if (currentComponent == "Renderer")
				{
					//add the coresponding asset to the library.
					assetLibrary->setArtAsset(aName, compElement->Attribute("sprite"));
					//add the component to the list
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::RendererComp);
				}
				else if (currentComponent == "Body")
				{
					//TODO: Consolidate all initial values into one struct
					PhysicsDevice::PhysicsStats physics;
					//get physics properties
					compElement->QueryFloatAttribute("density", &physics.density);
					compElement->QueryFloatAttribute("restitution", &physics.restitution);
					compElement->QueryFloatAttribute("angularDamping", &physics.angularDamping);
					compElement->QueryFloatAttribute("linearDamping", &physics.linearDamping);
					compElement->QueryBoolAttribute("physicsOn", &physics.physicsOn);
					std::string bodyType = compElement->Attribute("bodyType");
					std::string bodyShape = compElement->Attribute("bodyShape");

					//convert strings to enums
					if (bodyType == "ENGINE_DYNAMIC") { physics.bodyType = PhysicsDevice::BodyType::Dynamic; }
					else if (bodyType == "ENGINE_STATIC") { physics.bodyType = PhysicsDevice::BodyType::Static; }

					if (bodyShape == "ENGINE_RECTANGLE") { physics.bodyShape = PhysicsDevice::BodyShape::Rectangle; }
					else if (bodyShape == "ENGINE_CIRCLE") { physics.bodyShape = PhysicsDevice::BodyShape::Circle; }
					//add to library
					assetLibrary->setObjectPhysics(aName, physics);

					//add component to list
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::BodyComp);
				}
				else if (currentComponent == "Health")
				{
					// get the health
					ObjectFactory::ObjectFactoryPresets stats;
					compElement->QueryIntAttribute("health", (int*)&stats.health);
					//add to library
					assetLibrary->setObjectStats(aName, stats);
					//add component
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::HealthComp);
				}
				else if (currentComponent == "UserInput")
				{
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::UserInputComp);
				}
				// if we have a misspeleed or non-existant component name in the file
				else
				{
					std::cout << "INVALID component in assets.xml: " << currentComponent << std::endl;
					return false;
				}

				compElement = compElement->NextSiblingElement();
			}

			//Each asset should have at least one component!
			if (componentList.empty()) return false;

			//add to library
			assetLibrary->setComponentList(aName, componentList);

			//get the next Asset
			asset = asset->NextSiblingElement();
		} while (asset);
	}
	//========================================
	//Construct Sound Device
	//========================================
	deviceCreationElement = deviceCreationElement->NextSiblingElement();
	if (createThisDevice(deviceCreationElement))
	{
		//TODO:: grab file name from XML to load sounds into asset Library.
		sDevice = std::make_unique<SoundDevice>();
		if (!sDevice->initialize(this))
		{
			printf("Sound Device could not intialize!");
			exit(1);
		}

		//TODO::move to sound device!
		//*********************Load Sounds***************************
		//grab first sound
		tinyxml2::XMLElement* sounds = deviceCreationElement->FirstChildElement();
		while (sounds)
		{
			//get information from file
			std::string name = sounds->Attribute("name");
			std::string path = sounds->Attribute("path");

			bool background;
			sounds->QueryBoolAttribute("background", &background);

			//add to library based on whether it is background music or not.
			if (background)
			{
				assetLibrary->addBackgroundMusic(name, path);
			}
			else
			{
				assetLibrary->addSoundEffect(name, path);
			}
			sounds = sounds->NextSiblingElement();
		}

	}

	//***********************************************************
	
	//
	//set-up debugging
	//
	//Box2DDebugdraw* debugdraw = new Box2DDebugdraw();
	//debugdraw->Initialize(this);
 //   debugdraw->setFlags(b2draw::e_shapeBit | b2draw::e_aabbBit);  //Turn on shape (red color) and aabb (green) 

	////Add the Debug draw to the world
	//if(debugdraw!=NULL)
	//{
	//	pDevice -> getWorld() -> setDebugdraw(debugdraw);
	//}
	return true;
}

//**************************************
//Deletes all the devices.
//need to switch to smart pointers.
bool ResourceManager::shutdown()
//**************************************

{

	iDevice = nullptr;

	//gDevice->ShutDown();
	gDevice = nullptr;

	sDevice->Shutdown();
	sDevice = nullptr;

	pDevice = nullptr;


	assetLibrary = nullptr;

	factory = nullptr;



	return true;
}


