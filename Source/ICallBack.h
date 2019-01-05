#ifndef I_CALL_BACK_H
#define I_CALL_BACK_H

#include "EventHandler.h"

class ICallBack
{
public:
	virtual void triggered(EventHandler::EventData data = 0) = 0;
};
#endif // !I_CALL_BACK_H
/*

*/