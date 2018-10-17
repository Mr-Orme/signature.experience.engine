
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <map>
#include "Definitions.h"

class TiXmlElement;
class ResourceManager;
class Object;

class ObjectFactory
{
public:
	struct ObjectFactoryPresets
	{
		std::string objectType;
		Position position;
		EngineFloat angle;
		EngineInt walkSpeed;
		//TODO: need to update userInputComponent with this!
		EngineInt RUN_MULTIPLIER;
		EngineInt health;//move to new AssetLibraryGame
		EngineInt destructTime;//move to new AssetLibraryGame
		ResourceManager* devices;
		std::map<Direction, bool> gDirection;
		//std::shared_ptr<Object> joinedObject; //only needed for joint
	};

	Object* Create(ObjectFactoryPresets& presets);
	private:
	
};

#endif