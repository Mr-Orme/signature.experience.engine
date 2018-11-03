#include "ComponentsList.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "Object.h"
#include "AssetLibrary.h"

UserInputComponent::UserInputComponent(Object* owner):Component(owner){}
UserInputComponent::~UserInputComponent(){}

//**************************************
//set up some defaults
bool UserInputComponent::initialize(const ObjectFactory::ObjectFactoryPresets& presets)
{
	
	devices = presets.devices;

	for (int i = 0; i < (int)InputDevice::InputEvents::NUM_EVENTS; i++)
	{
		pressControl[(InputDevice::InputEvents)i] = true;
	}
	return true;
}
	
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
