//keymapping file
#ifndef USERINPUT_H
#define USERINPUT_H

#include "Component.h"
#include "Definitions.h"
#include "InputDevice.h"

class ResourceManager;

class UserInputComponent : public Component
{
public:
	UserInputComponent(Object* owner, ResourceManager* devices);
	~UserInputComponent();
	
	Object* update() final;
		
private:

	ResourceManager * devices{ nullptr };
	std::map<InputDevice::InputEvents, bool> pressControl;
	
	//TODO: This needs moved in with the map above. Not just a bool, but a struct of some sort. . .
	//May have to get further input
		const EngineInt BASE_FORCE_MULTIPLIER{ 0 };
		const EngineInt RUN_MULTIPLIER{ 0 }; //How many times faster is running then walking
	
};

#endif