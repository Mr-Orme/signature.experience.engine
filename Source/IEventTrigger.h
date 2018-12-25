#ifndef I_EVENT_TRIGGER_H
#define I_EVENT_TRIGGER_H
#include "EventHandler.h"
class ITrigger
{
public:
	EventHandler::Event eventToTrigger;
	virtual void triggerEvent(void* data)=0;
};
#endif // !I_EVENT_TRIGGER_H
