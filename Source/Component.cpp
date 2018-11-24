#include "Object.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(Object* owner) : owner(owner){}
//**************************************

Component::~Component(){}

//**************************************
//when the owner is destroyed, destructor will run afterwards.
void Component::OwnerDestroyed()
//**************************************
{
	owner = nullptr;
}

Object* Component::getOwner() const
{
	return(owner);
}