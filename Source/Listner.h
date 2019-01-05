#ifndef LISTNER_H
#define LISTNER_H

#include <vector>
#include "EventHandler.h"
class ICallBack;
class Listner
{
public:
	void addCallBack(ICallBack* callBack);
	void removeCallBack(const ICallBack* callBack);
	void eventTriggered(EventHandler::EventData data = 0);
private:
	std::vector<ICallBack*> callBacks;
};
#endif // !LISTNER_H
