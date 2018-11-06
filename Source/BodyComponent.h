#ifndef BODY_COMPONENT_H
#define BODY_COMPONENT_H

#include <memory>
#include "Component.h"
#include "Definitions.h"

class ResourceManager;
class Object;
class BodyComponent : public Component
{
public:
	BodyComponent(Object* owner, ResourceManager* devices, BodyPresets);
		
	void start() final;
	Object* update() final;
	void finish() final;

	//TODO:: add more functionality for other things Box2D can do.
	EngineFloat getAngle();
	Position getPosition();
	Position getVelocity();
	EngineInt getWidth();
	EngineInt getHeight();
	void setAngle(EngineFloat angle);
	void adjustAngle(EngineFloat adjustAmount);
	void linearStop();
	
	std::unique_ptr<BodyComponent> joinedWith{ nullptr };
private:
	ResourceManager * devices{ nullptr };
	
};

#endif