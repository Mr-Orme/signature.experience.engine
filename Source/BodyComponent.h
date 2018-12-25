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
	//Todo::add presets for callBack information
	BodyComponent(Object* owner, ResourceManager* devices, BodyPresets);
	//added destructor, deleted the finish method and copy pasted to destructor. deleted start and copy pasted to constructor.
	~BodyComponent();
		
	Object* update() final;

	//TODO:: add more functionality for other things Box2D can do.
	//Added more functionality and defined functions in cpp file
	//TODO:: you may need to add more functionality to the physicsDevice also!!!!
	
	/*MrOrme:: Need a way to do call backs that affect the body... Do I need one for each
	method call? Can I pass in a pointer to the method???
	*/
	EngineDefs::Float getAngle();

	EngineDefs::Vector getPosition();
	EngineDefs::Float getXPos();
	EngineDefs::Float getYPos();

	EngineDefs::Vector getVelocity();

	EngineDefs::Int getWidth();
	EngineDefs::Int getHeight();
	
	//TODO::change all params for setters to EngineDefs::Vector, then just grab the correct one
	//in the method. This allows for a generic function pointer for all setters in the bodyComponent
	//class.
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
	std::vector<std::unique_ptr<BodyCallBack>> callBacks;
	
};

#endif