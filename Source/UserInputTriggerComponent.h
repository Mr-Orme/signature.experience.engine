////keymapping file
#ifndef USERINPUT_H
#define USERINPUT_H
#include "Component.h"
#include "InputDevice.h"
#include "EventHandler.h"
#include "IEventTrigger.h"
class Object;
class ResourceManager;
class UserInputTriggerComponent : public Component, public ITrigger
{
public:
	UserInputTriggerComponent(Object* owner, ResourceManager* devices, InputDevice::UserInputs TriggeredInput);
	void triggerEvent(void* data) override;
	Object* update() override;
private:
	ResourceManager* devices;
	InputDevice::UserInputs TriggeredInput;


};
//
//#include "Component.h"
//#include "Definitions.h"
//#include "InputDevice.h"
//
//class ResourceManager;
//
//class UserInputTriggerComponent : public Component
//{
//public:
//	UserInputTriggerComponent(Object* owner, ResourceManager* devices);
//	~UserInputTriggerComponent();
//	
//	//no need for start function
//	Object* update() final;
//	//no need for finish function
//	struct Running {
//		const EngineDefs::Int BASE_FORCE_MULTIPLIER{ 0 };
//		const EngineDefs::Int RUN_MULTIPLIER{ 0 };
//	};
//		
//private:
//
//	ResourceManager * devices{ nullptr };
//	//Come back to this later
//	//turned this into a struct but not sure how to implement a struct with a map.
//	//UPDATE: put struct in public instead of private so we can reference in our map.
//	//std::map<InputDevice::UserInputs, UserInputTriggerComponent::Running> pressControl;
//	std::map<InputDevice::UserInputs, bool> pressControl;
//	//TODO: This needs moved in with the map above. Not just a bool, but a struct of some sort. . .
//	//May have to get further input
//		 //How many times faster is running then walking
//	
//};
//
#endif