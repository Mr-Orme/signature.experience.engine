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
	StatComponent(Object* owner, ResourceManager* devices, eInt health);
	~StatComponent() {};

	std::vector<std::unique_ptr<Object>> update() final;

	//TODO: move to eventManager when we have one. The event should check for a health stat component.
	//So there is a killObject method within our ResourceManager.h too, is it related?
	bool killObject(std::string &deathSprite);
	bool killObject();

	eInt getStat();
	std::string name;
	eInt statistic{ 0 };
	bool isDead{ false };
	std::unique_ptr<StatComponent> modifier{ nullptr };
private:
	
	ResourceManager* devices{ nullptr };
};

#endif