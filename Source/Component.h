#ifndef COMPONENT_H
#define COMPONENT_H

#include "Definitions.h"
#include "Initializers.h"

class Object;
class Component
{
public:
	Component(Object* owner);
	~Component();
	void OwnerDestroyed();
	Object* getOwner() const;
	//No need for pure virtual void start and finish functions.
	virtual Object* update()=0;
	
	bool initialized{ false };
protected:
	Object * owner{ nullptr };
	
};


#endif