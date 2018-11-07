#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;
//Todo:: change this to StatComponent that can keep track of any given stat. 
//Should have an int to keep track of the value and a string to keep track of the stat name.
//this will make it more generic.
//Killing the object will be implemented in a triggercomponent that is triggered by the stat reaching 0.
class HealthComponent : public Component
{
public:
	HealthComponent(Object* owner, ResourceManager* devices, EngineInt health);

	void start() final;
	Object* update() final;
	void finish() final;

	bool killObject(std::string deathSprite);
	bool killObject();

	EngineInt health{ 0 };
	bool isDead{ false };

private:
	
	ResourceManager* devices{ nullptr };
};

#endif