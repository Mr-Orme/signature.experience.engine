#include "ComponentsList.h"
#include "ResourceManager.h"
#include "Object.h"
#include "AssetLibrary.h"

UserInputComponent::UserInputComponent(Object* owner, ResourceManager* devices):Component(owner)
{
	this->devices = devices;

	for (int i = 0; i < (int)InputDevice::InputEvents::NUM_EVENTS; i++)
	{
		pressControl[(InputDevice::InputEvents)i] = true;
	}
}
UserInputComponent::~UserInputComponent(){}

void UserInputComponent::start(){}

//**************************************
//reacts to keyboard input and adjusts the world accoringly.
Object* UserInputComponent::update()
//**************************************
{
	for (auto key : devices->iDevice->keyStates)
	{
		if (key.second)
		{
			/*TODO: Based upon the key pressed, Some Event needs to happen! How to make this generic????
			Most likely some value on some object needs to be adjusted, or the view needs to be adjusted.
			This will probably tie in closely with the event manager.....
			*/
		}
	}
	return nullptr;
}

void UserInputComponent::finish(){}
