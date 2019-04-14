#ifndef NOTIFICATION_CALLBACK_H
#define NOTIFICATION_CALLBACK_H

#include <set>
#include <vector>
#include "SpriteComponent.h"
#include "ICallBack.h"

class IEventTrigger;
class Object;
class ResourceManager;
class NotificationCallBackComponent : public SpriteComponent, public ICallBack
{
public:
	/*
	1. some trigger is out in the world, and stored in set<EventData> triggers below
	2. Trigger notifies, eventhandler, passes itself
	3. eventHandler triggers all callBacks, passing on trigger.
	4. if, matches a trigger in set, then this is the trigger we are looking for, draw!
	*/
	NotificationCallBackComponent(Object* owner, ResourceManager* devices, tinyxml2::XMLElement* componentElement);
	Object* triggered(EventHandler::EventData data) override;
	void addTrigger(IEventTrigger* triggerforMe);
private:
	std::set<IEventTrigger*> triggers;
};
#endif // !NOTIFICATION_EVENT_H
