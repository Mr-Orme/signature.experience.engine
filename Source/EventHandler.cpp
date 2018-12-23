#include "EventHandler.h"
#include "Listner.h"

void EventHandler::notify(Event event)
{
	if (auto listnerIter = listners.find(event); listnerIter != listners.end())
	{
		listnerIter->second->eventTriggered();
	}
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
