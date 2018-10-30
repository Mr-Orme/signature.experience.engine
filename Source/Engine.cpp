#include "tinyxml2.h"

#include "Engine.h"
#include "Object.h"
#include "ResourceManager.h"
#include "ComponentsList.h"
#include "GraphicsDevice.h"
#include "View.h"
#include "Texture.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "AssetLibrary.h"
#include "Timer.h"

Engine::~Engine()
{
	if(engineDevices)
	{
		engineDevices -> shutdown();
		engineDevices = nullptr;
	}
}

//**************************************
//Initializes Engine & Art Asset Libraries.
bool Engine::initialize(std::string engineFile)
//**************************************
{	
	reset();
	
	//========================================
	//load the files
	//========================================
	tinyxml2::XMLDocument engineSettings;	
	
	if (!engineSettings.LoadFile(engineFile.c_str())){ return false; };
	
	tinyxml2::XMLElement* levelRoot = engineSettings.FirstChildElement();
	int screenWidth{ 0 };
	int screenHeight{ 0 };

	tinyxml2::XMLElement* currElement = levelRoot->FirstChildElement();
	currElement->QueryIntAttribute("width", &screenWidth);
	currElement->QueryIntAttribute("height", &screenHeight);

	//========================================
	//Construct Device Manager
	//========================================
	//TODO::Need a resource manager for the game
	mainScreen = std::make_unique<GraphicsDevice>(screenWidth, screenHeight);
	if (!mainScreen->initialize(true))
	{
		printf("Graphics Device could not Initialize!");
		exit(1);
	}
	RGBA fontColor{ 0,0,0,255 };
	mainScreen->setFont("./Assets/Fonts/impact.ttf", 16, fontColor);

	engineDevices = std::make_unique<ResourceManager>();
	engineDevices->initialize("./Assets/Config/engine.xml", mainScreen.get());

	///Get a few things ready
	ObjectFactory::ObjectFactoryPresets presets;
	presets.devices = engineDevices.get();
	ObjectFactory* objectFactory = engineDevices->factory.get();

	
	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Engine::run()
//**************************************
{

	//check to see if we have quit;
	if (engineDevices->iDevice->keyStates[InputDevice::InputEvents::QUIT] == true)
	{
		return false;
	}

	engineDevices->iDevice->update();

	//Construct Frame Timer
	std::unique_ptr<Timer> frameRate{new Timer()};
	if (!frameRate->Initialize(engineDevices->FPS))
	{
		printf("Frame Timer could not intialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//Start Frame Timer
	frameRate->start();

	update();
	draw();

	//pauses until proper refresh time has passed.
	frameRate->fpsRegulate();
	return true;
}
//**************************************
//call's the view's update method
//Call's each object's update method in the object vector.
void Engine::update()
//**************************************
{

	engineDevices->pDevice->update(1.0f / engineDevices->FPS);	 

	//clean out dead objects

	//TODO::Move to game part
	//for (auto objectIter = objects.begin(); objectIter != objects.end(); objectIter++)
	//{
	//	//check for health component
	//	HealthComponent* compHealth = (*objectIter)->getComponent<HealthComponent>();
	//	
	//	if (compHealth != nullptr && compHealth->isDead)
	//	{
	//		//**************Bring out your dead********************
	//		(*objectIter)->removeComponents();
	//		objects.erase(objectIter);
	//		objectIter--;
	//		//*******************************************************
	//	}
	//}


	//add any objects created in the previous iteration
	//TODO:: this needs to be in the game part.
	/*if (!newGameObjects.empty())
	{
		engineObjects.insert(engineObjects.end(), std::make_move_iterator(newObjects.begin()), std::make_move_iterator(newObjects.end()));
		newObjects.clear();
	}*/

	//Update objects.
	for (auto& object : engineObjects)
	{
		Object* temp = object->update();
		//TODO:: this needs to be in the game part.
	/*	if (temp != nullptr)
		{
			newGameObjects.push_back(std::unique_ptr<Object>(temp));
		}*/
	}
	//update view
	engineDevices->gDevice->getView()->update();
}

//**************************************
//Starts the graphic deivce, 
//calls all draw methods
//Then calls the graphic device's present method to show the frame.
void Engine::draw()
//**************************************
{
	engineDevices -> gDevice -> Begin();

	for (auto& object : engineObjects)
	{
		object->draw();
	}

	engineDevices -> gDevice -> draw();
	
	//if(debug) engineDevices -> pDevice -> getWorld() -> DrawDebugData();
	
	engineDevices -> gDevice -> Present();
}

//**************************************
//Clears the object vector, world, and view object in 
//preparation of loading a new level.
void Engine::reset()
//**************************************

{
	//if objects is not empty
	if (!engineObjects.empty())
	{
		//clear the vector
		engineObjects.clear();
	}
	//kill old Resource Manager;
	if (engineDevices) engineDevices->shutdown();
	engineDevices = nullptr;


}