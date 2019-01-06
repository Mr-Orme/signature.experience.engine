#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <memory>
#include "SDL.h"
#include "Definitions.h"


class InputDevice{
public:
	InputDevice();
	
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

	UserInputs Translate();
	std::unique_ptr<SDL_Event> event {nullptr}; 	
};


#endif