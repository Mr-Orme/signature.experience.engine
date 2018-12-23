#ifndef COMPONENT_H
#define COMPONENT_H

#include "Definitions.h"
#include "Initializers.h"

class Object;
class Component
{
public:
	Component(Object* owner);
	Component() = delete;
	~Component();

	Object* getOwner() const;

	virtual Object* update()=0;
	
protected:
	Object * owner{ nullptr };
	
};


#endif