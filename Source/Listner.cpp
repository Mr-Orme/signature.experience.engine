#include <algorithm>
#include <vector>
#include <memory>
#include "Listner.h"
#include "ICallBack.h"
#include "EventHandler.h"
#include "Object.h"

using namespace std;
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

vector<std::unique_ptr<Object>> Listner::eventTriggered(EventHandler::EventData data)
{
	vector<std::unique_ptr<Object>> temp;
	for (auto callBack : callBacks)
	{
		if (auto tempObj = callBack->triggered(data); tempObj != nullptr)
		{
			temp.push_back(std::unique_ptr<Object>(tempObj));
		}
	}
	return temp;
}
