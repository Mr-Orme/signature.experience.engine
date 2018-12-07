#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Definitions.h"
#include "SDL.h"

class InputDevice{
public:
	bool initialize();
	void update();
	
	enum class InputEvents
	{
		NA,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SPACE,
		SHIFT,
		B,
		QUIT,
		NUM_EVENTS
	};

	std::map<InputEvents, bool> keyStates;
private:
	InputEvents Translate(SDL_Event* event);
	SDL_Event* event {nullptr};
	
};


#endif