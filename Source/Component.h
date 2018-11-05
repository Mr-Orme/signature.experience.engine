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

	virtual void start()=0;
	virtual Object* update()=0;
	virtual void finish()=0;
	
	bool initialized{ false };
protected:
	Object * owner{ nullptr };
	
};


#endif