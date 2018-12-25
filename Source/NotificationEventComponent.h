#ifndef NOTIFICATION_EVENT_H
#define NOTIFICATION_EVENT_H

#include <set>
#include "SpriteComponent.h"
#include "ICallBack.h"

class IEventTrigger;
//#include "tinyxml2.h"
class Object;
class ResourceManager;
class NotificationEventComponent : public SpriteComponent, public ICallBack
{
public:
	/*
	1. some trigger is out in the world, and stored in set<void*> triggers below
	2. Trigger notifies, eventhandler, passes itself
	3. eventHandler triggers all callBacks, passing on trigger.
	4. if, matches a trigger in set, then this is the trigger we are looging for, draw!
	*/
	NotificationEventComponent(Object* owner, ResourceManager* devices, tinyxml2::XMLElement* componentElement);
	void triggered(void* data) override;
	void addTrigger(IEventTrigger* triggerforMe);
private:
	std::set<void*> triggers;
};
#endif // !NOTIFICATION_EVENT_H
