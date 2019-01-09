#include <algorithm>
#include "tinyxml2.h"

#include "NotificationCallBackComponent.h"
#include "IEventTrigger.h"

NotificationCallBackComponent::NotificationCallBackComponent(Object * owner, ResourceManager * devices, tinyxml2::XMLElement * componentElement)
	:SpriteComponent(owner, devices, componentElement)
{}

void NotificationCallBackComponent::triggered(EventHandler::EventData data)
{
	if (std::holds_alternative<IEventTrigger*>(data))
	{
		if (std::find(triggers.begin(), triggers.end(), std::get<IEventTrigger*>(data)) != triggers.end())
			draw();
	}
}

void NotificationCallBackComponent::addTrigger(IEventTrigger * triggerForMe)
{
	triggers.insert(triggerForMe);
}
