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
	
	//no need for start function
	Object* update() final;
	//no need for finish function
		
private:

	ResourceManager * devices{ nullptr };
	//Come back to this later
	std::map<InputDevice::InputEvents, UserInputComponent::Running> pressControl;
	struct Running{
		const EngineInt BASE_FORCE_MULTIPLIER{ 0 };
		const EngineInt RUN_MULTIPLIER{ 0 };
			};
	//TODO: This needs moved in with the map above. Not just a bool, but a struct of some sort. . .
	//May have to get further input
		 //How many times faster is running then walking
	
};

#endif