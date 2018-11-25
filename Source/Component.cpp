#include "Object.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(Object* owner) : owner(owner){}
//**************************************

Component::~Component(){}

//**************************************
//when the owner is destroyed, run the finish for the component
void Component::OwnerDestroyed()
//**************************************
{
	finish();
	owner = nullptr;
}

Object* Component::getOwner() const
{
	return(owner);
}