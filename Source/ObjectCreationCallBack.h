#ifndef OBJECT_CREATION_CALLBACK_H
#define OBJECT_CREATION_CALLBACK_H

#include <memory>
#include "ICallBack.h"
#include "Initializers.h"

class ObjectFactory;
class Object;
/*
	In XML file, when a trigger created for a Object Creation event, this is a child item that gets added to the event handler.
	It will need to have everything it needs for the initializers list.
	
*/
class ObjectCreationCallBack: public ICallBack
{
public:
	ObjectCreationCallBack() = delete;
	//~ObjectCreationCallBack();
	ObjectCreationCallBack(ObjectFactory* objectFactory, std::unique_ptr<ObjectFactoryPresets> presets);
	Object* triggered(EventHandler::EventData data = {}) override;

private:
	ObjectFactory* objectFactory{ nullptr };
	std::unique_ptr<ObjectFactoryPresets> presets;
};



#endif // !OBJECT_CREATION_CALLBACK_H

