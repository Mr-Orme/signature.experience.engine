#include <iostream>
#include "InputDevice.h"

//**************************************
//sets all keystates to false. sets initial event to NA
//NA should be passed in, but can be any valid event.

InputDevice::InputDevice()
{
	//========================================
	//Construct Event System
	//========================================
	event = new SDL_Event();
	if (!event) {
		printf("SDL Event could not initialize!");
		exit(1);
	}

	keyStates[UserInputs::NA] = false;
	keyStates[UserInputs::DOWN] = false;
	keyStates[UserInputs::UP] = false;
	keyStates[UserInputs::LEFT] = false;
	keyStates[UserInputs::RIGHT] = false;
	keyStates[UserInputs::SPACE] = false;
	keyStates[UserInputs::QUIT] = false;
	keyStates[UserInputs::SHIFT] = false;
	keyStates[UserInputs::B] = false;
	update();
}

//**************************************
//updates the event based on the passed in SDL_EVENT
void InputDevice::update()
//**************************************
{
	UserInputs gEvent;
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
			keyStates.find(UserInputs::QUIT) -> second = true;
			break;
		default:
			break;
		}
	}
}

//**************************************
//converts the SDL event to a game event
InputDevice::UserInputs InputDevice::Translate(SDL_Event* event)
//**************************************
{
	//This switch is here in case we want to add other events, such as mouse events.
	switch(event->key.keysym.sym)
		{
		case SDLK_LEFT:
			return UserInputs::LEFT;
			break;
		case SDLK_RIGHT:
			return UserInputs::RIGHT;
			break;
		case SDLK_UP:
			return UserInputs::UP;
			break;
		case SDLK_DOWN:
			return UserInputs::DOWN;
			break;	
		case SDLK_SPACE:
			return UserInputs::SPACE;
			break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return UserInputs::SHIFT;
			break;
		case SDLK_b:
			return UserInputs::B;
			break;
		}

	return UserInputs::NA;
}
