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
	//added destructor
	~BodyComponent();
		
	Object* update() final;
	void finish() final;

	//TODO:: add more functionality for other things Box2D can do.
	EngineFloat getAngle();
	Position getPosition();
	Position getVelocity();
	EngineInt getWidth();
	EngineInt getHeight();
	//added setVelocity and individual X and Y Velocity
	void setVelocity(Position velocity);
	void setXVelocity(EngineFloat xVel);
	void setYVelocity(EngineFloat yVel);
	void setAngle(EngineFloat angle);
	//Added changePosition and ability to increase speed. Maybe a float to get current speed and x y positions
	void changePosition(Position position);
	void increaseForwardVelocity(EngineFloat num);
	void decreaseForwardVelocity(EngineFloat num);
	float getXPos();
	float getYPos();
	float getSpeed();
	void adjustAngle(EngineFloat adjustAmount);
	void linearStop();
	
	std::unique_ptr<BodyComponent> joinedWith{ nullptr };
private:
	ResourceManager * devices{ nullptr };
	
};

#endif