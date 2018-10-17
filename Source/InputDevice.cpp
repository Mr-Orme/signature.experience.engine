#include <iostream>
#include "InputDevice.h"

//**************************************
//sets all keystates to false. sets initial event to NA
//NA should be passed in, but can be any valid event.
bool InputDevice::initialize()
//**************************************
{
	//========================================
	//Construct Event System
	//========================================
	event = new SDL_Event();
	if(!event){
		printf( "SDL Event could not initialize!");
		exit(1);
	}

	keyStates[ENGINE_NA] = false;
	keyStates[ENGINE_DOWN] = false;
	keyStates[ENGINE_UP] = false;
	keyStates[ENGINE_LEFT] = false;
	keyStates[ENGINE_RIGHT] = false;
	keyStates[ENGINE_SPACE] = false;
	keyStates[ENGINE_QUIT] = false;
	keyStates[ENGINE_SHIFT] = false;
	keyStates[ENGINE_B] = false;
	update();
	return true;
}

//**************************************
//updates the event based on the passed in SDL_EVENT
void InputDevice::update()
//**************************************
{
	InputEvents gEvent;
	if(SDL_PollEvent(event))
	{
		//updates the proper key state based on the event that was passed in
		switch (event -> type)
		{
		case SDL_KEYDOWN:
			//translates the SDL even to a game event.
			gEvent = Translate(event);
			keyStates.find(gEvent) -> second = true;
			break;
		case SDL_KEYUP:
			//translates the SDL even to a game event.
			gEvent = Translate(event);
			keyStates.find(gEvent) -> second = false;
			break;
		case SDL_QUIT:
			keyStates.find(ENGINE_QUIT) -> second = true;
			break;
		default:
			break;
		}
	}
	
	
	
}

//**************************************
//converts the SDL event to a game event
InputDevice::InputEvents InputDevice::Translate(SDL_Event* event)
//**************************************
{
	//This switch is here in case we want to add other events, such as mouse events.
	switch(event->key.keysym.sym)
		{
		case SDLK_LEFT:
			return ENGINE_LEFT;
			break;
		case SDLK_RIGHT:
			return ENGINE_RIGHT;
			break;
		case SDLK_UP:
			return ENGINE_UP;
			break;
		case SDLK_DOWN:
			return ENGINE_DOWN;
			break;	
		case SDLK_SPACE:
			return ENGINE_SPACE;
			break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return ENGINE_SHIFT;
			break;
		case SDLK_b:
			return ENGINE_B;
			break;
		}



	return ENGINE_NA;
}

//**************************************
//returns the state of a key
bool InputDevice::getEvent(InputEvents event)
//**************************************
{
	return (keyStates.find(event) -> second);
}
