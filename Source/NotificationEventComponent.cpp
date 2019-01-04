#include <algorithm>
#include "tinyxml2.h"

#include "NotificationEventComponent.h"
#include "IEventTrigger.h"

NotificationEventComponent::NotificationEventComponent(Object * owner, ResourceManager * devices, tinyxml2::XMLElement * componentElement)
	:SpriteComponent(owner, devices, componentElement)
{}

void NotificationEventComponent::triggered(void* data)
{
	if(std::find(triggers.begin(), triggers.end(), (IEventTrigger*)data)!=triggers.end())
		draw();
}

void NotificationEventComponent::addTrigger(IEventTrigger * triggerforMe)
{
	triggers.insert(triggerforMe);
}
