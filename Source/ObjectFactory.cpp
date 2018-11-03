#include <iostream>
#include <string>
#include "ObjectFactory.h"
#include "Component.h"
#include "ComponentsList.h"
#include "View.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Object.h"
#include "AssetLibrary.h"

using namespace std;

Object* objectFromComponentList(
	const std::vector<AssetLibrary::AssetLibraryComponentList> enumComponents, 
	const ObjectFactory::ObjectFactoryPresets& presets)
{

	
	//Create pointer to new objects
	Object* newObject = new Object();

	std::vector<Component*> componentList;
	//Iterate through the list of components
	//add the proper component to the list of components to return.
	for (auto comp : enumComponents)
	{
		switch (comp)
		{
		case AssetLibrary::AssetLibraryComponentList::BodyComp:
			componentList.push_back(new BodyComponent(newObject));
			break;
		case AssetLibrary::AssetLibraryComponentList::HealthComp:
			componentList.push_back(new HealthComponent(newObject));
			break;
		case AssetLibrary::AssetLibraryComponentList::SpriteComp:
			componentList.push_back(new SpriteComponent(newObject));
			break;
		case AssetLibrary::AssetLibraryComponentList::UserInputComp:
			componentList.push_back(new UserInputComponent(newObject));
			break;
		default:
			break;
		}
	}
	for (auto comp : componentList)
	{
		newObject->AddComponent(comp);
	}

	//initialize the new object (this will also initialize components)
	if (newObject->initialize(presets))
	{
		//return the created and initialized object.
		return newObject;
	}
	else
	{
		delete newObject;
		std::cout << "Object did not initialize!" << std::endl;
		return nullptr;
	}
}
//************************************************
//ALL ITEMS IN "presets" need to be set.
//based on the object type, it grabs components.
// and adds them to a newly created object.
//it initializes the object which also initializes all the components
Object* ObjectFactory::Create(ObjectFactory::ObjectFactoryPresets& presets)
//**************************************
{
	//get list of components for the new object
	return objectFromComponentList(presets.devices -> assetLibrary ->getComponents(presets.objectType), presets);
	//Add each to the object	
}

Object * ObjectFactory::Create(tinyxml2::XMLElement * objectElement)
{
	std::vector<AssetLibrary::AssetLibraryComponentList> enumComponents;
	ObjectFactory::ObjectFactoryPresets presets;
	presets.objectType = objectElement->Attribute("type");
	//MrOrme:: Need to finish initializing components!
	for (
		tinyxml2::XMLElement* componentElement = objectElement->FirstChildElement("Component");
		componentElement;
		componentElement = componentElement->NextSiblingElement("Component")
		)
	{
		string componentName = componentElement->Attribute("name");
		if (componentName == "Sprite")
		{
			enumComponents.push_back(AssetLibrary::AssetLibraryComponentList::SpriteComp);
		}
		else if (componentName == "Body")
		{
			enumComponents.push_back(AssetLibrary::AssetLibraryComponentList::BodyComp);
		}
		else if (componentName == "Health")
		{
			enumComponents.push_back(AssetLibrary::AssetLibraryComponentList::HealthComp);

		}
		else if (componentName == "Input")
		{
			enumComponents.push_back(AssetLibrary::AssetLibraryComponentList::UserInputComp);

		}
		
	}

	return objectFromComponentList(presets.devices->assetLibrary->getComponents(presets.objectType), presets);
}

