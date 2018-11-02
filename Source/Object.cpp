//#include "ComponentsList.h"
#include "Object.h"
#include "SpriteComponent.h"

 Object:: Object()
{
	initialized = false;
}

 Object::~ Object()
{

}

//**************************************
//get's the components up and running based on passed presests
bool  Object::initialize(ObjectFactory::ObjectFactoryPresets& presets)
//**************************************
{
	objectType = presets.objectType;
	joinedWith = nullptr;
	
	//the renderer component needs to be initialized first because the body component depends on it
	//It's initialization method has a check to see if it is already initialized 
	//so that when we go through all the components and initialize them in the next step, it won't do it again.
	auto component = getComponent<SpriteComponent>();
	if(component) component -> initialize(presets);

	//initialize all components
	for ( auto& component : components)
	{
	
		
		component-> initialize(presets);
	}
	//start all components
	if(!initialized)
	{
		for (auto& component : components)
		{
			component -> start();
		}
		initialized = true;
	}
	return true;
}

//**************************************
//Adds the passed component to the object.
void  Object::AddComponent(Component* component)
//**************************************
{
	components.push_back(std::unique_ptr<Component>(component));
}

//**************************************
//runs the update method for all attached components
 Object* Object::update()
//**************************************

{
	 Object* newObject = nullptr;
	for (auto& component : components)
	{
		if( auto tempObject = component ->update(); tempObject != nullptr)
		{
			newObject = tempObject;
		}
			
	}
	return newObject;
}
void  Object::draw()
{
	if (SpriteComponent* sprite = getComponent<SpriteComponent>(); sprite != nullptr)
	{
		sprite->draw();
	}
}
//**************************************
//takes off a compoent and runs its finsih method
bool  Object::removeComponents()
//**************************************
{
	
	for (auto& comp: components)
	{
		comp -> finish();
		
	}
	components.clear();
	return true;
}

