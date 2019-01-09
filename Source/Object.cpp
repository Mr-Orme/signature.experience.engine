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
	//Todo::MrOrme need to not draw if notification component...
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

