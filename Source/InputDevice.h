#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Definitions.h"
#include "SDL.h"

class InputDevice{
public:
	InputDevice();
	//Put initialize into constructor.
	void update();
	
	enum class UserInputs
	{
		NA,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SPACE,
		SHIFT,
		MOUSE_MOVE,
		LEFT_CLICK,
		RIGHT_CLICK,
		DOUBLE_CLICK,
		B,
		QUIT,
		NUM_EVENTS
	};

	std::map<UserInputs, bool> keyStates;
private:
	UserInputs Translate(SDL_Event* event);
	SDL_Event* event {nullptr};
	
};


#endif