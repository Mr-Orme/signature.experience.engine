#ifndef I_CALL_BACK_H
#define I_CALL_BACK_H

#include "EventHandler.h"
class Object;
class ICallBack
{
public:
	ICallBack();
	~ICallBack();
	virtual Object* triggered(EventHandler::EventData data = {}) = 0;
};
#endif // !I_CALL_BACK_H
/*

*/