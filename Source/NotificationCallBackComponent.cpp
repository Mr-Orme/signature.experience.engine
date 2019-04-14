#include <algorithm>
#include "tinyxml2.h"

#include "NotificationCallBackComponent.h"
#include "IEventTrigger.h"

NotificationCallBackComponent::NotificationCallBackComponent(Object * owner, ResourceManager * devices, tinyxml2::XMLElement * componentElement)
	:SpriteComponent(owner, devices, componentElement)
{}

Object* NotificationCallBackComponent::triggered(EventHandler::EventData data)
{
	
	if (std::holds_alternative<IEventTrigger*>(data))
	{
		//checks that the triggering event is one of the events that triggers this notification.
		if (std::find(triggers.begin(), triggers.end(), std::get<IEventTrigger*>(data)) != triggers.end())
			draw();
	}
	return nullptr;
}

void NotificationCallBackComponent::addTrigger(IEventTrigger * triggerForMe)
{
	triggers.insert(triggerForMe);
}
