#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;

class HealthComponent : public Component
{
public:
	HealthComponent(Object* owner);

	bool initialize(const ObjectFactoryPresets& presets) final;
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