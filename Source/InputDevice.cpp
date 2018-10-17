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

	keyStates[InputEvents::NA] = false;
	keyStates[InputEvents::DOWN] = false;
	keyStates[InputEvents::UP] = false;
	keyStates[InputEvents::LEFT] = false;
	keyStates[InputEvents::RIGHT] = false;
	keyStates[InputEvents::SPACE] = false;
	keyStates[InputEvents::QUIT] = false;
	keyStates[InputEvents::SHIFT] = false;
	keyStates[InputEvents::B] = false;
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
			keyStates.find(InputEvents::QUIT) -> second = true;
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
			return InputEvents::LEFT;
			break;
		case SDLK_RIGHT:
			return InputEvents::RIGHT;
			break;
		case SDLK_UP:
			return InputEvents::UP;
			break;
		case SDLK_DOWN:
			return InputEvents::DOWN;
			break;	
		case SDLK_SPACE:
			return InputEvents::SPACE;
			break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return InputEvents::SHIFT;
			break;
		case SDLK_b:
			return InputEvents::B;
			break;
		}

	return InputEvents::NA;
}
