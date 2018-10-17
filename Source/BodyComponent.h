#ifndef BODY_COMPONENT_H
#define BODY_COMPONENT_H

#include "Component.h"
#include "Definitions.h"
#include "ObjectFactory.h"

class ResourceManager;
class Object;
class BodyComponent : public Component
{
public:
	BodyComponent(Object* owner);
		
	bool initialize(ObjectFactory::ObjectFactoryPresets& presets) final;
	void start() final;
	Object* update() final;
	void finish() final;

	ENGINE_FLT getAngle();
	Position getPosition();
	Position getVelocity();
	EngineInt getWidth();
	EngineInt getHeight();
	void setAngle(ENGINE_FLT angle);
	void adjustAngle(ENGINE_FLT adjustAmount);
	void linearStop();
	
private:
	ResourceManager * devices{ nullptr };
};

#endif