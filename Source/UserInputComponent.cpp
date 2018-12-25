
#include "ComponentsList.h"
#include "ResourceManager.h"
#include "Object.h"
#include "AssetLibrary.h"

//UserInputComponent::UserInputComponent(Object* owner, ResourceManager* devices):Component(owner)
//{
//	this->devices = devices;
//
//	for (int i = 0; i < (int)InputDevice::UserInputs::NUM_EVENTS; i++)
//	{
//		pressControl[(InputDevice::UserInputs)i] = true;
//	}
//}
//UserInputComponent::~UserInputComponent(){}
//
////**************************************
////reacts to keyboard input and adjusts the world accoringly.
//Object* UserInputComponent::update()
////**************************************
//{
//	for (auto key : devices->iDevice->keyStates)
//	{
//		if (key.second)
//		{
//			/*TODO: Based upon the key pressed, Some Event needs to happen! How to make this generic????
//			Most likely some value on some object needs to be adjusted, or the view needs to be adjusted.
//			This will probably tie in closely with the event manager.....
//			*/
//			//Again the event manager sounds like the ResourceManager.
//			//maybe you need an owner or some kind of reference pointer that will be modified.
//		}
//	}
//	return nullptr;
//}

UserInputComponent::UserInputComponent(Object * owner, ResourceManager * devices, InputDevice::UserInputs input):
	Component(owner), 
	devices(devices),
	TriggeredInput(input)
{
	eventToTrigger = EventHandler::Event::UserInput;
}

void UserInputComponent::triggerEvent(void* data)
{
	devices->eventHandler->notify(eventToTrigger, data);
}

Object * UserInputComponent::update()
{
	if (auto inputTrigger = devices->iDevice->keyStates.find(TriggeredInput); 
	inputTrigger != devices->iDevice->keyStates.end() && inputTrigger ->second)
	{
		triggerEvent((void*)&inputTrigger->first);
	}
	return nullptr;
}
