#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <memory>
#include <map>
#include <string>
#include "SDL.h"
#include "Definitions.h"
#include "Vector2D.h"


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
	Vector2D getMousePosition();
	UserInputs text2Input(std::string inputString);
	std::map<UserInputs, bool> keyStates;
private:
	Vector2D mousePosition{ 0,0 };
	UserInputs keyTranslate();
	std::unique_ptr<SDL_Event> event {nullptr}; 	
};


#endif