#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <vector>

#include "Definitions.h"
#include "ObjectFactory.h"
#include "Component.h"

class GraphicsDevice;
class PhysicsDevice;
class BackpackComponent;
class BodyComponent;
class GhostComponent;
class HealthComponent;
class InventoryComponent;
class RendererComponent;
class UserInputComponent;

class Object
{
public:
	 Object();
	~Object();
	
	bool initialize(ObjectFactory::ObjectFactoryPresets& presets);
	void AddComponent(Component* component);
	Object* update();
	void draw();

	void setJoinedWith(Object* joinedWith) { this->joinedWith = joinedWith; }

	std::string getObjectType() { return objectType; }
	Object* getJoinedWith() { return joinedWith; }

	bool removeComponents();

	template<class T>
	T* getComponent()
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
	std::string objectType{ " " };
	//TODO: Needs to move to body component!!!
	Object* joinedWith{ nullptr };

};

#endif

