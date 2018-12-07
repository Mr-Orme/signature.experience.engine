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
	//added destructor, deleted the finish method and copy pasted to destructor. deleted start and copy pasted to constructor.
	~BodyComponent();
		
	Object* update() final;

	//TODO:: add more functionality for other things Box2D can do.
	//Added more functionality and defined functions in cpp file
	EngineFloat getAngle();
	Position getPosition();
	Position getVelocity();
	EngineInt getWidth();
	EngineInt getHeight();
	//added setVelocity and individual X and Y Velocity
	//Defined functions in the cpp file
	void setVelocity(Position velocity);
	void setXVelocity(EngineFloat xVel);
	void setYVelocity(EngineFloat yVel);
	void setAngle(EngineFloat angle);
	//Added changePosition and ability to increase speed. Maybe a float to get current speed and x y positions
	//Changed changePosition to setPosition
	void setPosition(Position position);
	void increaseForwardVelocity(EngineFloat num);
	void decreaseForwardVelocity(EngineFloat num);
	float getXPos();
	float getYPos();
	//Commented out getSpeed since this is a scalar and not a vector.
	//float getSpeed();
	void adjustAngle(EngineFloat adjustAmount);
	void linearStop();
	
	std::unique_ptr<BodyComponent> joinedWith{ nullptr };
private:
	ResourceManager * devices{ nullptr };
	
};

#endif