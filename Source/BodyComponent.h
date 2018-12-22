#ifndef BODY_COMPONENT_H
#define BODY_COMPONENT_H

#include "Component.h"
#include "Definitions.h"

class ResourceManager;
class Object;
class BodyComponent : public Component
{
public:
	BodyComponent(Object* owner, ResourceManager* devices, BodyPresets);
	//added destructor, deleted the finish method and copy pasted to destructor. deleted start and copy pasted to constructor.
	~BodyComponent();
		
	Object* update() final;

	//TODO:: add more functionality for other things Box2D can do.
	//Added more functionality and defined functions in cpp file
	EngineDefs::Float getAngle();

	EngineDefs::Vector getPosition();
	EngineDefs::Float getXPos();
	EngineDefs::Float getYPos();

	EngineDefs::Vector getVelocity();

	EngineDefs::Int getWidth();
	EngineDefs::Int getHeight();
	
	void setVelocity(EngineDefs::Vector velocity);
	void setXVelocity(EngineDefs::Float value);
	void setYVelocity(EngineDefs::Float value);
	
	//
	void accelerate(EngineDefs::Float force);
	void deccelerate(EngineDefs::Float force);

	void setAngle(EngineDefs::Float angle);
	void rotate(EngineDefs::Float degrees);

	void setPosition(EngineDefs::Vector position);

	
	
	void linearStop();
	
	std::unique_ptr<BodyComponent> joinedWith{ nullptr };
private:
	ResourceManager * devices{ nullptr };
	
};

#endif