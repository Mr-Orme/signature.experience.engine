#include "NotificationEventComponent.h"
#include "tinyxml2.h"

NotificationEventComponent::NotificationEventComponent(Object * owner, ResourceManager * devices, tinyxml2::XMLElement * componentElement):SpriteComponent(owner, devices, componentElement)
{

}

void NotificationEventComponent::triggered()
{
	draw();
}
