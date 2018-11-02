
#include <iostream>

#include "ResourceManager.h"
#include "ComponentsList.h"
#include "Object.h"

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

using namespace std;
inline bool createThisDevice(tinyxml2::XMLElement* createMe)
{
	bool create;
	createMe->QueryBoolAttribute("create", &create);
	return create;
}
//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
bool ResourceManager::initialize(std::string assetPath)
{
	tinyxml2::XMLDocument levelConfig;
	if (!levelConfig.LoadFile(assetPath.c_str())) { return false; };
		
	tinyxml2::XMLElement* levelRoot = levelConfig.FirstChildElement();//Level
	tinyxml2::XMLElement* currElement = levelRoot->FirstChildElement();//Screen
	
	int screenWidth{ 0 };
	int screenHeight{ 0 };
	currElement->QueryIntAttribute("width", &screenWidth);
	currElement->QueryIntAttribute("height", &screenHeight);

	//========================================
	//Construct Device Manager
	//========================================
	gDevice = std::make_unique<GraphicsDevice>(screenWidth, screenHeight);
	if (!gDevice->initialize(true))
	{
		printf("Graphics Device could not Initialize!");
		exit(1);
	}
	
	if (tinyxml2::XMLElement* fontConfig = currElement->FirstChildElement(); fontConfig)
	{
		std::unique_ptr<RGBA> fontColor = std::make_unique<RGBA>();
			
		fontConfig->QueryIntAttribute("R", (int*)&fontColor.get()->R);
		fontConfig->QueryIntAttribute("G", (int*)&fontColor.get()->G);
		fontConfig->QueryIntAttribute("B", (int*)&fontColor.get()->B);
		fontConfig->QueryIntAttribute("A", (int*)&fontColor.get()->A);
		
		int fontSize;
		fontConfig->QueryIntAttribute("size", &fontSize);
		
		string fontPath = fontConfig->Attribute("path");
		
		gDevice->setFont(fontPath, fontSize, *fontColor);		
	}
	
	currElement = currElement->NextSiblingElement();//FPS
	FPS = std::stoi(currElement->GetText());
	
	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_unique<ObjectFactory>();
	
	currElement = currElement->NextSiblingElement();//Devices
	
	
	//========================================
	//Construct Input Device
	//========================================
	tinyxml2::XMLElement* deviceConfig = currElement->FirstChildElement("Input");
	if (createThisDevice(deviceConfig))
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
	deviceConfig = deviceConfig->NextSiblingElement("Physics");
	if (createThisDevice(deviceConfig))
	{
		Position gravity{ 0, 0 };
		deviceConfig->QueryFloatAttribute("gravityX", &gravity.x);
		deviceConfig->QueryFloatAttribute("gravityY", &gravity.y);
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
	deviceConfig = deviceConfig->NextSiblingElement("AssetLibrary");
	if (createThisDevice(deviceConfig))
	{
		assetLibrary = std::make_unique<AssetLibrary>();
		if (!assetLibrary->initialize(sDevice.get(), gDevice.get())) { exit(1); }

		tinyxml2::XMLElement* asset = deviceConfig->FirstChildElement("Asset");

		while (asset)
		{
			std::string assetName = asset->Attribute("name");

			std::vector<AssetLibrary::AssetLibraryComponentList> componentList;

			tinyxml2::XMLElement* compElement = asset->FirstChildElement("Component");
			while (compElement)
			{
				std::string currentComponent = compElement->Attribute("name");

				if (currentComponent == "Renderer")
				{
					assetLibrary->setArtAsset(assetName, compElement->Attribute("sprite"));
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::RendererComp);
				}
				else if (currentComponent == "Body")
				{
					PhysicsDevice::PhysicsStats physics;
					compElement->QueryFloatAttribute("density", &physics.density);
					compElement->QueryFloatAttribute("restitution", &physics.restitution);
					compElement->QueryFloatAttribute("angularDamping", &physics.angularDamping);
					compElement->QueryFloatAttribute("linearDamping", &physics.linearDamping);
					compElement->QueryBoolAttribute("physicsOn", &physics.physicsOn);
					std::string bodyType = compElement->Attribute("bodyType");
					std::string bodyShape = compElement->Attribute("bodyShape");

					if (bodyType == "ENGINE_DYNAMIC") { physics.bodyType = PhysicsDevice::BodyType::Dynamic; }
					else if (bodyType == "ENGINE_STATIC") { physics.bodyType = PhysicsDevice::BodyType::Static; }

					if (bodyShape == "ENGINE_RECTANGLE") { physics.bodyShape = PhysicsDevice::BodyShape::Rectangle; }
					else if (bodyShape == "ENGINE_CIRCLE") { physics.bodyShape = PhysicsDevice::BodyShape::Circle; }

					assetLibrary->setObjectPhysics(assetName, physics);
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::BodyComp);
				}
				else if (currentComponent == "Health")
				{
					ObjectFactory::ObjectFactoryPresets stats;
					compElement->QueryIntAttribute("health", (int*)&stats.health);
					assetLibrary->setObjectStats(assetName, stats);
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::HealthComp);
				}
				else if (currentComponent == "UserInput")
				{
					componentList.push_back(AssetLibrary::AssetLibraryComponentList::UserInputComp);
				}
				else
				{
					std::cout << "INVALID component in xml: " << currentComponent << std::endl;
					return false;
				}

				compElement = compElement->NextSiblingElement("Component");
			}

			if (componentList.empty()) return false;

			assetLibrary->setComponentList(assetName, componentList);

			asset = asset->NextSiblingElement("Asset");
		} 
	}
	//========================================
	//Construct Sound Device
	//========================================
	deviceConfig = deviceConfig->NextSiblingElement("Sound");
	if (createThisDevice(deviceConfig))
	{
		sDevice = std::make_unique<SoundDevice>();
		if (!sDevice->initialize(this))
		{
			printf("Sound Device could not intialize!");
			exit(1);
		}

		//TODO::move all sounds to sound device!
		//*********************Load Sounds***************************
		//grab first sound
		tinyxml2::XMLElement* sounds = deviceConfig->FirstChildElement("SoundEffect");
		while (sounds)
		{
			//get information from file
			std::string name = sounds->Attribute("name");
			std::string path = sounds->Attribute("path");

			bool background;
			sounds->QueryBoolAttribute("background", &background);

			if (background)
			{
				assetLibrary->addBackgroundMusic(name, path);
			}
			else
			{
				assetLibrary->addSoundEffect(name, path);
			}
			sounds = sounds->NextSiblingElement("SoundEffect");
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
	if (!objects.empty())
	{
		objects.clear();
	}
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

void ResourceManager::update()
{
	iDevice->update();
	pDevice->update(1.0f / FPS);

	for (auto objectIter = objects.begin(); objectIter != objects.end(); )
	{
		if (
			HealthComponent* compHealth = (*objectIter)->getComponent<HealthComponent>(); 
			compHealth != nullptr && compHealth->isDead)
		{
			//**************Bring out your dead********************
			(*objectIter)->removeComponents();
			objects.erase(objectIter);
			//*******************************************************
		}
		else
		{
			objectIter++;
		}
	}
	for (auto& object : objects)
	{
		Object* temp = object->update();
		if (temp != nullptr)
		{
			newObjects.push_back(std::unique_ptr<Object>(temp));
		}
	}

	if (!newObjects.empty())
	{
		objects.insert(objects.end(), std::make_move_iterator(newObjects.begin()), std::make_move_iterator(newObjects.end()));
		newObjects.clear();
	}

	gDevice->getView()->update();
}

void ResourceManager::draw()
{
	gDevice->Begin();

	for (auto& object : objects)
	{
		object->draw();
	}

	gDevice->draw();

	gDevice->Present();
}


