#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include "Definitions.h"
#include "Component.h"

class GraphicsDevice;
class PhysicsDevice;
class BackpackComponent;
class BodyComponent;
class GhostComponent;
class HealthComponent;
class InventoryComponent;
class SpriteComponent;
class UserInputTriggerComponent;

class Object
{
public:
	//Todo::Josh::add move constructor, copy constructor, and = move constructor to this!
	 Object();
	~Object();
	
	void AddComponent(Component* component);
	std::vector<std::unique_ptr<Object>> update();
	void draw();
	
	bool removeComponents();

	template<class T>
	T* getComponent() const
	{
		for (auto& component : components)
		{
			
			if (T* target = nullptr; target = dynamic_cast<T*>(component.get()))
			{
				return(target);
			}
		}
		return(nullptr);
	}

private:
	
	std::vector<std::unique_ptr<Component>> components;
	bool initialized{ false };

};

#endif

