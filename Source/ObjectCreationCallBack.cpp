#include <memory>
#include "ObjectCreationCallBack.h"
#include "ObjectFactory.h"
#include "Initializers.h"

ObjectCreationCallBack::ObjectCreationCallBack(ObjectFactory * objectFactory, std::unique_ptr<ObjectFactoryPresets> presets) : objectFactory(objectFactory), presets(std::move(presets))
{
	
}

Object* ObjectCreationCallBack::triggered(EventHandler::EventData data)
{

	
	return objectFactory->create(presets.get());
}
