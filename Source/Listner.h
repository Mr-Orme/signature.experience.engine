#ifndef LISTNER_H
#define LISTNER_H

#include <vector>
#include "EventHandler.h"
class ICallBack;
class Object;
class Listner
{
public:
	void addCallBack(ICallBack* callBack);
	void removeCallBack(const ICallBack* callBack);
	std::vector<std::unique_ptr<Object>> eventTriggered(EventHandler::EventData data = {});
private:
	std::vector<ICallBack*> callBacks;
};
#endif // !LISTNER_H
