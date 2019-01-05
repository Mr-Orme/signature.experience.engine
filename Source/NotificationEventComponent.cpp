#include <algorithm>
#include "tinyxml2.h"

#include "NotificationEventComponent.h"
#include "IEventTrigger.h"

NotificationEventComponent::NotificationEventComponent(Object * owner, ResourceManager * devices, tinyxml2::XMLElement * componentElement)
	:SpriteComponent(owner, devices, componentElement)
{}

void NotificationEventComponent::triggered(EventHandler::EventData data)
{
	if (std::holds_alternative<IEventTrigger*>(data))
	{
		if (std::find(triggers.begin(), triggers.end(), std::get<IEventTrigger*>(data)) != triggers.end())
			draw();
	}
}

void NotificationEventComponent::addTrigger(IEventTrigger * triggerForMe)
{
	triggers.insert(triggerForMe);
}
