#include "tinyxml2.h"

#include "Engine.h"
#include "Object.h"
#include "ResourceManager.h"
#include "ComponentsList.h"
#include "GraphicsDevice.h"
#include "ViewCallBack.h"
#include "Texture.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "AssetLibrary.h"
#include "Timer.h"

using namespace std;

Engine::~Engine()
{
	if(engineDevices)
	{
		engineDevices = nullptr;
	}
}

//**************************************
//Initializes Engine & Art Asset Libraries.
bool Engine::initialize()
//**************************************
{	
	reset();
	engineDevices = make_unique<ResourceManager>("./Assets/Config/engine.xml");
	
	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Engine::run()
//**************************************
{

	//check to see if we have quit;
	if (engineDevices->iDevice->keyStates[InputDevice::UserInputs::QUIT])
	{
		return false;
	}

	//Construct Frame Timer
	std::unique_ptr<Timer> frameRate{new Timer()};
	if (!frameRate->Initialize(engineDevices->FPS))
	{
		printf("Frame Timer could not intialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//Start Frame Timer
	frameRate->start();

	engineDevices->update();
	engineDevices->draw();

	//pauses until proper refresh time has passed.
	frameRate->fpsRegulate();
	return true;
}

//**************************************
//Clears the object vector, world, and view object in 
//preparation of loading a new level.
void Engine::reset()
//**************************************

{
	//kill old Resource Manager;
	engineDevices = nullptr;


}