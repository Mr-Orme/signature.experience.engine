#ifndef USERINPUT_H
#define USERINPUT_H
#include <vector>
#include "Component.h"
#include "InputDevice.h"
#include "EventHandler.h"
#include "IEventTrigger.h"
class Object;
class ResourceManager;
class UserInputTriggerComponent : public Component, public ITrigger
{
public:
	UserInputTriggerComponent(
		Object* owner,
		ResourceManager* devices,
		InputDevice::UserInputs TriggeredInput,
		EventHandler::Event = EventHandler::Event::UserInput);
	std::vector<std::unique_ptr<Object>> triggerEvent(EventHandler::EventData data) override;
	std::vector<std::unique_ptr<Object>> update() override;
private:
	ResourceManager* devices;
	InputDevice::UserInputs TriggeredInput;
	bool canBeTriggered{ true };


};

#endif