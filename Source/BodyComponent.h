#ifndef BODY_COMPONENT_H
#define BODY_COMPONENT_H

#include <vector>
#include "Component.h"
#include "Definitions.h"


class BodyCallBack;
class ResourceManager;
class Object;
class BodyComponent : public Component
{
public:
	BodyComponent(Object* owner, ResourceManager* devices, BodyPresets presets);
	~BodyComponent();
		
	Object* update() final;

	//TODO::Josh::add more functionality for other things Box2D can do.
	//Josh::Added more functionality and defined functions in cpp file
	//TODO::Josh::you may need to add more functionality to the physicsDevice also!!!!
	
	eFloat getAngle();

	Vector2D getPosition();
	eFloat getXPos();
	eFloat getYPos();

	Vector2D getVelocity();

	eInt getWidth();
	eInt getHeight();
	
	//TODO::Josh::change all params for these methods to Vector2D, then just grab the correct one
	//in the method. This allows for a generic function pointer for all setters in the bodyComponent
	//class.
	void setVelocity(Vector2D velocity);
	void setXVelocity(eFloat value);
	void setYVelocity(eFloat value);
	
	void accelerate(eFloat force);
	void deccelerate(eFloat force);

	void setAngle(eFloat angle);
	void rotate(eFloat degrees);

	void setPosition(Vector2D position);	
	
	void linearStop();
	
	std::unique_ptr<BodyComponent> joinedWith{ nullptr };
private:
	ResourceManager * devices{ nullptr };
	std::vector<std::unique_ptr<BodyCallBack>> callBacks;
	
};

#endif