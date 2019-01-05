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
	void triggerEvent(EventHandler::EventData data) override;
	Object* update() override;
private:
	ResourceManager* devices;
	InputDevice::UserInputs TriggeredInput;


};

#endif