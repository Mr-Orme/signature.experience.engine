
#include <iostream>
#include "ResourceManager.h"

#include "InputDevice.h"
#include "GraphicsDevice.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"

#include "AssetLibrary.h"
#include "ObjectFactory.h"

#include "Definitions.h"
#include "tinyxml2.h"

//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
bool ResourceManager::Initialize(EngineInt SCREEN_WIDTH, EngineInt SCREEN_HEIGHT, std::string assetPath)
{

	/*TODO: Create Level class and have vector of levels. . . . Level needs:
		1. Physics World - make swappable worlds in physics device
		2. Objects vector
	*/

	//TODO: XML of game values including:
	//1. FPS
	//2. Screen dimensions
	FPS = 100;
	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_unique<ObjectFactory>();


	//========================================
	//Construct Input Device
	//========================================
	iDevice = std::make_unique<InputDevice>();
	if(!iDevice->initialize())
	{
		printf( "Input Device could not initialize!");
		exit(1);
	}

	//========================================
	//Construct Graphical Device
	//========================================
	gDevice = std::make_unique<GraphicsDevice>(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!gDevice->initialize(true))
	{
		printf( "Graphics Device could not Initialize!");
		exit(1);
	}
	//color of fonts
	RGBA fontColor{ 0,0,0,255 };
	
	//set graphic device font
	gDevice -> setFont("./Assets/Fonts/impact.ttf", 16, fontColor);

	//========================================
	//Construct Physics Device
	//========================================
	pDevice = std::make_unique<PhysicsDevice>(0,0);
		
	if(!pDevice -> initialize())
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}

	//========================================
	//Construct Sound Device
	//========================================
	sDevice = std::make_unique<SoundDevice>();

	if (!sDevice->initialize(this))
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}

	//========================================
	//Construct Asset Library
	//========================================
	assetLibrary = std::make_unique<AssetLibrary>();
	//needs to be an xml file just likes physics.
	if (!assetLibrary->initialize(sDevice.get(), gDevice.get())) { exit(1); }
	


	//TODO: Update to tinyXML2!!!
	//prepare XML file for parsing.
	tinyxml2::XMLDocument assetFile;
	
	if(!assetFile.LoadFile(assetPath.c_str())){return false;};
	tinyxml2::XMLElement* level = assetFile.FirstChildElement();
	tinyxml2::XMLElement* assetType = level->FirstChildElement();
	//get first asset
	tinyxml2::XMLElement* asset = assetType -> FirstChildElement();
	//add each asset into each library.
	do
	{
		//get the name of the asset
		std::string aName = asset -> Attribute("name");
	
		//where we will store the components.
		std::vector<AssetLibrary::AssetLibraryComponentList> componentList;
		
		//move to the components of the xml
		tinyxml2::XMLElement* compElement = asset -> FirstChildElement();
		
		//Add each component to the vector
		while (compElement)
		{
			//get the component's name
			std::string currentComponent = compElement -> Attribute("name");
			//find the component we just grabbed
			if(currentComponent == "Renderer")
			{
				//add the coresponding asset to the library.
				assetLibrary -> setArtAsset(aName, compElement -> Attribute("sprite"));
				//add the component to the list
				componentList.push_back(AssetLibrary::AssetLibraryComponentList::RendererComp);
			}
			else if(currentComponent == "Body")
			{
				//TODO: Consolidate all initial values into one struct
				PhysicsDevice::PhysicsStats physics;
				//get physics properties
				compElement -> QueryFloatAttribute("density", &physics.density);
				compElement -> QueryFloatAttribute("restitution", &physics.restitution);
				compElement -> QueryFloatAttribute("angularDamping", &physics.angularDamping);
				compElement -> QueryFloatAttribute("linearDamping", &physics.linearDamping);
				compElement -> QueryBoolAttribute("physicsOn", &physics.physicsOn);
				std::string bodyType;
				std::string bodyShape;
				compElement -> QueryStringAttribute("bodyType", &(bodyType.data()));
				compElement -> QueryStringAttribute("bodyShape", &bodyShape);
		
				//convert strings to enums
				if(bodyType == "ENGINE_DYNAMIC"){physics.bodyType = PhysicsDevice::BodyType::Dynamic;}
				else if (bodyType == "ENGINE_STATIC"){physics.bodyType = PhysicsDevice::BodyType::Static;}
		
				if(bodyShape == "ENGINE_RECTANGLE"){physics.bodyShape = PhysicsDevice::BodyShape::Rectangle;}
				else if (bodyShape == "ENGINE_CIRCLE"){physics.bodyShape = PhysicsDevice::BodyShape::Circle;}
				//add to library
				assetLibrary -> setObjectPhysics(aName, physics);
				
				//add component to list
				componentList.push_back(AssetLibrary::AssetLibraryComponentList::BodyComp);
			}
			else if(currentComponent == "Health")
			{
				// get the health
				ObjectFactory::ObjectFactoryPresets stats;
				compElement -> QueryIntAttribute("health", (int*)&stats.health);
				//add to library
				assetLibrary -> setObjectStats(aName, stats);
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

			compElement = compElement -> NextSiblingElement();
		}
		
		//Each asset should have at least one component!
		if(componentList.empty()) return false;
		
		//add to library
		assetLibrary -> setComponentList(aName, componentList);
		
		//get the next Asset
		asset = asset -> NextSiblingElement();
	} while (asset);
	
	//*********************Load Notices***************************
	//move to notice section of file
	assetType = assetType -> NextSiblingElement();
	//grab first notice
	tinyxml2::XMLElement* notices = assetType -> FirstChildElement();
	do
	{
		//get information from file
		AssetLibrary::Notice notice;
		int x,y;
		char* direction = new char[1];
		notices -> QueryIntAttribute("x", &x);
		notices -> QueryIntAttribute("y", &y);
		notices -> QueryStringAttribute("direction", &direction);
		notice.text = notices ->GetText();
		
		//store in notice, in proper format.
		if(direction == "N") notice.direction = N;
		else if(direction == "E") notice.direction = E;
		else if(direction == "S") notice.direction = S;
		else if(direction == "W") notice.direction = W;
		
		//add it to the library.
		assetLibrary -> setNotice(notice);
		
		//get the next notice
		notices = notices -> NextSiblingElement();
	} while (notices);
	//***********************************************************

	//*********************Load Sounds***************************
	//move tos ound section of file
	assetType = assetType -> NextSiblingElement();
	//grab first sound
	tinyxml2::XMLElement* sounds = assetType -> FirstChildElement();
	while (sounds)
	{
		//get information from file
		std::string name;
		sounds -> QueryStringAttribute("name", &name);
		std::string path;
		sounds -> QueryStringAttribute("path", &path);
		bool background;
		sounds -> QueryBoolAttribute("background", &background);

		//add to library based on whether it is background music or not.
		if(background)
		{
			assetLibrary -> addBackgroundMusic(name, path);
		}
		else
		{
			assetLibrary -> addSoundEffect(name, path);
		}
		sounds = sounds -> NextSiblingElement();
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
bool ResourceManager::Shutdown()
//**************************************

{

	iDevice = nullptr;

	gDevice->ShutDown();
	gDevice = nullptr;

	sDevice->Shutdown();
	sDevice = nullptr;

	pDevice = nullptr;


	assetLibrary = nullptr;

	factory = nullptr;



	return true;
}


