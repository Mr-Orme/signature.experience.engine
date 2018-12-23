#ifndef NOTIFICATION_EVENT_H
#define NOTIFICATION_EVENT_H

#include "SpriteComponent.h"
#include "ICallBack.h"
//#include "tinyxml2.h"
class Object;
class ResourceManager;
class NotificationEventComponent : public SpriteComponent, public ICallBack
{
public:
	NotificationEventComponent(Object* owner, ResourceManager* devices, tinyxml2::XMLElement* componentElement);
	void triggered() override;
};
#endif // !NOTIFICATION_EVENT_H
