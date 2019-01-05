#ifndef NOTIFICATION_EVENT_H
#define NOTIFICATION_EVENT_H

#include <set>
#include "SpriteComponent.h"
#include "ICallBack.h"

class IEventTrigger;
class Object;
class ResourceManager;
class NotificationEventComponent : public SpriteComponent, public ICallBack
{
public:
	/*
	1. some trigger is out in the world, and stored in set<EventData> triggers below
	2. Trigger notifies, eventhandler, passes itself
	3. eventHandler triggers all callBacks, passing on trigger.
	4. if, matches a trigger in set, then this is the trigger we are looking for, draw!
	*/
	NotificationEventComponent(Object* owner, ResourceManager* devices, tinyxml2::XMLElement* componentElement);
	void triggered(EventHandler::EventData data) override;
	void addTrigger(IEventTrigger* triggerforMe);
private:
	std::set<IEventTrigger*> triggers;
};
#endif // !NOTIFICATION_EVENT_H
