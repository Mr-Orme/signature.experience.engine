#include <vector>
#include "EventHandler.h"
#include "Listner.h"
#include "Object.h"

using namespace std;
EventHandler::EventHandler()
{
	for (int currEvent = 0; currEvent < (int)Event::NumEvents; currEvent++)
	{
		listners.insert_or_assign((Event)currEvent, std::make_unique<Listner>());
	}
}

EventHandler::~EventHandler()
{
}

vector<std::unique_ptr<Object>> EventHandler::notify(Event event, EventHandler::EventData data)
{
	
	vector<std::unique_ptr<Object>> toBeReturned;
	if (auto listnerIter = listners.find(event); listnerIter != listners.end())
	{
		auto temp = listnerIter->second->eventTriggered(data);
		toBeReturned.insert(toBeReturned.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
	}
	return toBeReturned;
}

Listner * EventHandler::getListner(Event event)
{
	auto listner = listners.find(event);
	return listner != listners.end() ? listner->second.get() : nullptr;
}

void EventHandler::addListner(Event event)
{
	listners.insert_or_assign(event, std::make_unique<Listner>());
}

void EventHandler::removeListner(Event event)
{
	if (auto listner = listners.find(event); listner != listners.end())
	{
		listners.erase(listner);
	}
}
