#include "ViewCallBack.h"
#include "Object.h"
#include "ResourceManager.h"
#include "Listner.h"
#include "EventHandler.h"

View::View(Vector2D position, ResourceManager* devices):position(position)
{
	devices->eventHandler->getListner(EventHandler::Event::AdjustView)->addCallBack(this);
}

Object* View::triggered(EventHandler::EventData data)
{
	//adjusts view based on triggered event. Possibly userInput, player location, or time lapse.
	if(std::holds_alternative<Vector2D>(data))
	{
		position += std::get<Vector2D>(data);
	}
	return nullptr;
}


