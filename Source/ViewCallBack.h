#ifndef VIEWCALLBACK_H
#define VIEWCALLBACK_H
#include "Definitions.h"
#include "ICallBack.h"


class ResourceManager;
class View: public ICallBack
{
public:
	View() = delete;
	View(Vector2D position, ResourceManager* devices);

	Object* triggered(EventHandler::EventData data) override;

	Vector2D position;

};


#endif