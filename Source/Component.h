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

	//deleted pure virtual function start
	virtual Object* update()=0;
	//deleted pure virtual function finish
	
	bool initialized{ false };
protected:
	Object * owner{ nullptr };
	
};


#endif