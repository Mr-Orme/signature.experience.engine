
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H
#include <memory>
#include "Definitions.h"
#include "Initializers.h"
#include "tinyxml2.h"

struct ObjectFactoryPresets;
class ResourceManager;
class Object;

class ObjectFactory
{
public:
	ObjectFactory() = delete;
	ObjectFactory(ResourceManager* devices);
	Object* create(tinyxml2::XMLElement* objectElement);
	Object* create(ObjectFactoryPresets* presets) ;
private:
	ResourceManager* devices{ nullptr };
	std::unique_ptr<ObjectFactoryPresets> createPresetsFromXML(tinyxml2::XMLElement* objectElement);
	void addSpritePresets(SpritePresets* presets, tinyxml2::XMLElement * componentElement);
	void addBodyPresets(BodyPresets* presets, tinyxml2::XMLElement * componentElement);
	void addUserInputPresets(UserInputPresets* presets, tinyxml2::XMLElement * componentElement);
	void addJointPresets(Joints* presets, tinyxml2::XMLElement * componentElement);
	void addSteeringPresets(SteeringPresets* presets, tinyxml2::XMLElement * componentElement);

};

#endif