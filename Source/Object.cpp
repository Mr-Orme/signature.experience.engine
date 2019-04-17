//#include "ComponentsList.h"
#include "Object.h"
#include "SpriteComponent.h"
#include "NotificationCallBackComponent.h"

 Object:: Object()
{
	initialized = false;
}

 Object::~Object()
{

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
std::vector<std::unique_ptr<Object>> Object::update()
//**************************************

{
	std::vector<std::unique_ptr<Object>> newObjects;
	for (auto& component : components)
	{
		if( auto temp = component ->update(); temp.size() > 0)
		{
			newObjects.insert(newObjects.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
		}
			
	}
	return newObjects;
}
void  Object::draw()
{
	if (getComponent <NotificationCallBackComponent>() == nullptr)
	{
		if (SpriteComponent* sprite = getComponent<SpriteComponent>(); sprite != nullptr)
		{
			sprite->draw();
		}
	}
}
//**************************************
//takes off a compoent and runs its finsih method
bool  Object::removeComponents()
//**************************************
{
	
	for (auto& comp: components)
	{
		comp -> ~Component();
		
	}
	components.clear();
	return true;
}

