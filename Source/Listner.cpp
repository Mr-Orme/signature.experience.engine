#include <algorithm>
#include "Listner.h"
#include "ICallBack.h"
#include "EventHandler.h"

void Listner::addCallBack(ICallBack * callBack)
{
	callBacks.push_back(callBack);
}

void Listner::removeCallBack(const ICallBack * callBack)
{
	if	(
			auto toRemove = std::find(callBacks.begin(), callBacks.end(), callBack); 
			toRemove != callBacks.end()
		)
	{
		callBacks.erase(toRemove);
	}
}

void Listner::eventTriggered(EventHandler::EventData data)
{
	for (auto callBack : callBacks)
	{
		callBack->triggered(data);
	}
}
