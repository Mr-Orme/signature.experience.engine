
#include <iostream>
#include <memory>
#include "tinyxml2.h"

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


using namespace std;
inline bool createThisDevice(tinyxml2::XMLElement* createMe)
{
	bool create;
	createMe->QueryBoolAttribute("create", &create);
	return create;
}
ResourceManager::ResourceManager(std::string assetPath)
{
	tinyxml2::XMLDocument levelConfig;
	if (!levelConfig.LoadFile(assetPath.c_str()) == tinyxml2::XML_SUCCESS)
	{
		std::cout << "Error, Problem in loading levelConfig file. Please check to see if valid file." << std::endl;
	};

	tinyxml2::XMLElement* levelRoot = levelConfig.FirstChildElement();//Level
	tinyxml2::XMLElement* levelElement = levelRoot->FirstChildElement();//Screen

	int screenWidth{ 0 };
	int screenHeight{ 0 };
	levelElement->QueryIntAttribute("width", &screenWidth);
	levelElement->QueryIntAttribute("height", &screenHeight);

	//========================================
	//Construct Device Manager
	//========================================
	gDevice = std::make_unique<GraphicsDevice>(screenWidth, screenHeight);
	if (!gDevice->initialize(true))
	{
		printf("Graphics Device could not Initialize!");
		exit(1);
	}

	if (tinyxml2::XMLElement* fontConfig = levelElement->FirstChildElement(); fontConfig)
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

	levelElement = levelElement->NextSiblingElement();//FPS
	FPS = std::stoi(levelElement->GetText());

	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_unique<ObjectFactory>(this);

	levelElement = levelElement->NextSiblingElement();//Devices


													  //========================================
													  //Construct Input Device
													  //========================================
	tinyxml2::XMLElement* deviceConfig = levelElement->FirstChildElement("Input");
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
		//TODO::initialize in constructor for all devices. Have initialized member that we check!
		assetLibrary = std::make_unique<AssetLibrary>();
		if (!assetLibrary->initialize(this)) { exit(1); }

		//*********************Load sprites***************************
		tinyxml2::XMLElement* asset = deviceConfig->FirstChildElement("Asset");

		while (asset)
		{
			assetLibrary->addArtAsset(asset->Attribute("name"), asset->Attribute("spritePath"));
			asset = asset->NextSiblingElement("Asset");
		}


	}


	//========================================
	//Construct Sound Device
	//========================================
	deviceConfig = deviceConfig->NextSiblingElement("Sound");
	if (createThisDevice(deviceConfig))
	{
		sDevice = std::make_unique<SoundDevice>(assetLibrary.get());
		if (!sDevice->getInitialized())
		{
			printf("Sound Device could not intialize!");
			exit(1);
		}
		//*********************Load Sounds***************************
		tinyxml2::XMLElement* sounds = deviceConfig->FirstChildElement("SoundEffect");
		while (sounds)
		{
			bool background;
			sounds->QueryBoolAttribute("background", &background);

			if (background)
			{
				assetLibrary->addBackgroundMusic(sounds->Attribute("name"), sounds->Attribute("path"));
			}
			else
			{
				assetLibrary->addSoundEffect(sounds->Attribute("name"), sounds->Attribute("path"));
			}
			sounds = sounds->NextSiblingElement("SoundEffect");
		}
	}


	//========================================
	//Construct Objects
	//========================================
	for (
		levelElement = levelElement->NextSiblingElement("Object");
		levelElement;
		levelElement = levelElement->NextSiblingElement("Object")
		)
	{
		objects.push_back(std::unique_ptr<Object>(factory->Create(levelElement)));
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
}
ResourceManager::~ResourceManager()
{
	if (!objects.empty())
	{
		objects.clear();
	}
	iDevice = nullptr;
	sDevice = nullptr;
	gDevice = nullptr;
	pDevice = nullptr;
	assetLibrary = nullptr;
	factory = nullptr;
}
//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
//This is handled in Constructor
/*bool ResourceManager::initialize(std::string assetPath)
{
	
}*/

//**************************************
//Deletes all the devices.
//need to switch to smart pointers.
//This is handled in destructor
/*bool ResourceManager::shutdown()
//**************************************

{
	
}*/

void ResourceManager::update()
{
	iDevice->update();
	pDevice->update(1.0f / FPS);

	for (auto objectIter = objects.begin(); objectIter != objects.end(); )
	{
		if (
			//Changed HealthComponent to StatComponent
			StatComponent* compHealth = (*objectIter)->getComponent<StatComponent>(); 
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

bool ResourceManager::killObject(Object * butAScratch)
{
	if (butAScratch)
	{
		//set up the object for death.
	}
}


