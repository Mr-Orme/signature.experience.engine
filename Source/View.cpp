#include "View.h"
#include "Object.h"
#include "ResourceManager.h"
#include "Listner.h"
#include "EventHandler.h"

View::View(EngineDefs::Vector position, ResourceManager* devices):position(position)
{
	devices->eventHandler->getListner(EventHandler::Event::AdjustView)->addCallBack(this);
}

void View::triggered(EventHandler::EventData data)
{
	if(std::holds_alternative<EngineDefs::Vector>(data))
	{
		position += std::get<EngineDefs::Vector>(data);
	}
}



//TODO: create a view adjust event. View should be a child of the ICallBack class!

