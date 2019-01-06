#ifndef VIEW_H
#define VIEW_H
#include "Definitions.h"
#include "ICallBack.h"


class ResourceManager;
class View: public ICallBack
{
public:
	View() = delete;
	View(EngineDefs::Vector position, ResourceManager* devices);

	void triggered(EventHandler::EventData data) override;

	EngineDefs::Vector position;

};


#endif