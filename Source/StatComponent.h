#ifndef STAT_H
#define STAT_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;
//Todo:: change this to StatComponent that can keep track of any given stat. 
//Should have an int to keep track of the value and a string to keep track of the stat name.
//this will make it more generic.
//Killing the object will be implemented in a triggercomponent that is triggered by the stat reaching 0.
class StatComponent : public Component
{
public:
	StatComponent(Object* owner, ResourceManager* devices, EngineInt health);

	void start() final;
	Object* update() final;
	void finish() final;

	bool killObject(std::string deathSprite);
	bool killObject();
	//Added a string to handle status effects and added EngineInt to handle changes due to status Effects.
	std::string statusEffect = " ";
	EngineInt buffOrNerf{ 0 };
	EngineInt health{ 0 };
	bool isDead{ false };

private:
	
	ResourceManager* devices{ nullptr };
};

#endif