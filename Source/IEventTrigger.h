#ifndef I_EVENT_TRIGGER_H
#define I_EVENT_TRIGGER_H
#include <vector>
#include "EventHandler.h"

class Object;
class ITrigger
{
public:
	EventHandler::Event eventToTrigger;
	virtual std::vector<std::unique_ptr<Object>> triggerEvent(EventHandler::EventData data)=0;
};
#endif // !I_EVENT_TRIGGER_H
