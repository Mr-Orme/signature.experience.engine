
#include "ComponentsList.h"
#include "ResourceManager.h"
#include "Object.h"
#include "AssetLibrary.h"
using namespace std;
//UserInputTriggerComponent::UserInputTriggerComponent(Object* owner, ResourceManager* devices):Component(owner)
//{
//	this->devices = devices;
//
//	for (int i = 0; i < (int)InputDevice::UserInputs::NUM_EVENTS; i++)
//	{
//		pressControl[(InputDevice::UserInputs)i] = true;
//	}
//}
//UserInputTriggerComponent::~UserInputTriggerComponent(){}
//
////**************************************
////reacts to keyboard input and adjusts the world accoringly.
//Object* UserInputTriggerComponent::update()
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

UserInputTriggerComponent::UserInputTriggerComponent(Object * owner, ResourceManager * devices, InputDevice::UserInputs input, EventHandler::Event triggeringEvent):
	Component(owner), 
	devices(devices),
	TriggeredInput(input)
{
	eventToTrigger = triggeringEvent;
}

vector<std::unique_ptr<Object>> UserInputTriggerComponent::triggerEvent(EventHandler::EventData data)
{
	return devices->eventHandler->notify(eventToTrigger, data);
}

vector<std::unique_ptr<Object>> UserInputTriggerComponent::update()
{
	std::vector<std::unique_ptr<Object>> toBeReturned;
	if (auto inputTrigger = devices->iDevice->keyStates.find(TriggeredInput); 
	inputTrigger != devices->iDevice->keyStates.end()) 
	{
		if (inputTrigger->second)
		{
			if (canBeTriggered)
			{
				toBeReturned = triggerEvent(inputTrigger->first);
				canBeTriggered = false;
			}
		}
		else
		{
			canBeTriggered = true;
		}
	}

	return toBeReturned;
}
