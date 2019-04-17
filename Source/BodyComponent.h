#ifndef BODY_COMPONENT_H
#define BODY_COMPONENT_H

#include <vector>
#include "Component.h"
#include "Definitions.h"

#include "Vector2D.h"

class BodyCallBack;
class ResourceManager;
class Object;
class BodyComponent : public Component
{
public:
	BodyComponent(Object* owner, ResourceManager* devices, BodyPresets& presets);
	~BodyComponent();
		
	std::vector<std::unique_ptr<Object>> update() final;

	//TODO::Josh::add more functionality for other things Box2D can do.
	//Josh::Added more functionality and defined functions in cpp file
	//TODO::Josh::you may need to add more functionality to the physicsDevice also!!!!
	
	eFloat getAngle();

	Vector2D getPosition();
	const eFloat maxSpeed = 5;
	Vector2D getVelocity();

	eInt getWidth();
	eInt getHeight();
	
	//TODO::Josh::change all params for these methods to Vector2D, then just grab the correct one
	//in the method. This allows for a generic function pointer for all setters in the bodyComponent
	//class.
	void setVelocity(Vector2D velocity);	
	void accelerate(Vector2D force);
	void deccelerate(Vector2D force);
	void applyForce(Vector2D forceVec);

	void setAngle(Vector2D angle);
	//void applyRotationalForce(Vector2D degrees);

	void setPosition(Vector2D position);	
	
	void linearStop(Vector2D unused);
	
	std::unique_ptr<BodyComponent> joinedWith{ nullptr };
private:
	ResourceManager * devices{ nullptr };
	std::vector<std::unique_ptr<BodyCallBack>> callBacks;
	
};

#endif