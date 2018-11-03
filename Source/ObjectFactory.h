
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <map>
#include "Definitions.h"
#include "tinyxml2.h"

class ResourceManager;
class Object;

class ObjectFactory
{
public:
	struct ObjectFactoryPresets
	{
		std::string objectType{ "" };
		Position position{ 0,0 };
		EngineFloat angle{ 0.0f };
		EngineInt walkSpeed{ 0 };
		//TODO: need to update userInputComponent with this!
		EngineInt RUN_MULTIPLIER{ 0 };
		EngineInt health{ 0 };//move to new AssetLibraryGame
		EngineInt destructTime{ 0 };//move to new AssetLibraryGame
		ResourceManager* devices{ nullptr };
		std::map<Direction, bool> gDirection;
		std::string textString{ "" };
		std::string vAlign{ "" };
		std::string hAlign{ "" };

		//std::shared_ptr<Object> joinedObject; //only needed for joint
	};

	Object* Create(ObjectFactoryPresets& presets);
	Object* Create(tinyxml2::XMLElement* objectElement);
	private:
	
};

#endif