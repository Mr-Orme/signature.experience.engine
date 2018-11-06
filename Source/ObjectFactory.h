
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <map>
#include "Definitions.h"
#include "tinyxml2.h"

struct ObjectFactoryPresets;
class ResourceManager;
class Object;

class ObjectFactory
{
public:
	ObjectFactory() = delete;
	ObjectFactory(ResourceManager* devices);
	Object* Create(tinyxml2::XMLElement* objectElement);
private:
	ResourceManager* devices{ nullptr };
};

#endif