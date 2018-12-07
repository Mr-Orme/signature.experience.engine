#include "Object.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(Object* owner) : owner(owner){}
//**************************************

Component::~Component(){}

//**************************************
//when the owner is destroyed, run the finish for the component
//UPDATE: finish is deleted so destructor will run after owner is destroyed.
void Component::OwnerDestroyed()
//**************************************
{
	owner = nullptr;
}

Object* Component::getOwner() const
{
	return(owner);
}