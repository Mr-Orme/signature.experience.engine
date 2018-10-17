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
	if(devices)
	{
		devices -> shutdown();
		devices = nullptr;
	}
}

//**************************************
//Initializes Engine & Art Asset Libraries.
bool Engine::initialize(std::string engineFile)
//**************************************
{
	//========================================
	//Base Engine Constants
	//========================================
	SCREEN_WIDTH = 1024;
	SCREEN_HEIGHT = 768;
	
	reset();
	//========================================
	//Construct Device Manager
	//========================================
	devices = std::make_unique<ResourceManager>();
	//devices->initialize(SCREEN_WIDTH, SCREEN_HEIGHT, assetConfigFile);
	
	///Get a few things ready
	ObjectFactory::ObjectFactoryPresets presets;
	presets.devices = devices.get();
	ObjectFactory* objectFactory = devices->factory.get();

	//========================================
	//load the files
	//========================================
	tinyxml2::XMLDocument currentLevel;	
	
	//if (!currentLevel.LoadFile(levelConfig.c_str())){ return false; };
	
	tinyxml2::XMLElement* levelRoot = currentLevel.FirstChildElement();
	//TODO: Decide default level XML config
	

	devices->sDevice->setBackground("main");

	//Successfully loaded level
	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Engine::run()
//**************************************
{

	//check to see if we have quit;
	if (devices->iDevice->keyStates[InputDevice::InputEvents::QUIT] == true)
	{
		return false;
	}

	devices->iDevice->update();

	//Construct Frame Timer
	std::unique_ptr<Timer> frameRate{new Timer()};
	if (!frameRate->Initialize(devices->FPS))
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

	devices->pDevice->update(1.0f / devices->FPS);	 

	//clean out dead objects
	for (auto objectIter = objects.begin(); objectIter != objects.end(); objectIter++)
	{
		//check for health component
		HealthComponent* compHealth = (*objectIter)->getComponent<HealthComponent>();
		
		if (compHealth != nullptr && compHealth->isDead)
		{
			//**************Bring out your dead********************
			(*objectIter)->removeComponents();
			objects.erase(objectIter);
			objectIter--;
			//*******************************************************
		}
	}


	//add any objects created in the previous iteration
	if (!newObjects.empty())
	{
		objects.insert(objects.end(), std::make_move_iterator(newObjects.begin()), std::make_move_iterator(newObjects.end()));
		newObjects.clear();
	}

	//Update objects.
	for (auto& object : objects)
	{
		Object* temp = object->update();
		
		if (temp != nullptr)
		{
			newObjects.push_back(std::unique_ptr<Object>(temp));
		}
	}
	//update view
	devices->gDevice->getView()->update();
}

//**************************************
//Starts the graphic deivce, 
//Graphic Device draws the sprites
//Then calls the graphic device's present method to show the frame.
void Engine::draw()
//**************************************
{
	devices -> gDevice -> Begin();

	for (auto& object : objects)
	{
		object->draw();
	}

	devices -> gDevice -> draw();
	
	//if(debug) devices -> pDevice -> getWorld() -> DrawDebugData();
	
	devices -> gDevice -> Present();
}

//**************************************
//Clears the object vector, world, and view object in 
//preparation of loading a new level.
void Engine::reset()
//**************************************

{
	//if objects is not empty
	if (!objects.empty())
	{
		//for every object in objects
		for (const auto& object : objects)
		{
			//remove it from the physics world
			//FIXME::this should not be necessary, should happen when the body component dies. . ..
			devices->pDevice->removeObject(object.get());
		}
		//clear the vector
		objects.clear();
	}
	//kill old Resource Manager;
	if (devices) devices->shutdown();
	devices = nullptr;


}