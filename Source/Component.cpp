#include "Object.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(Object* owner) : owner(owner){}
//**************************************

Component::~Component(){}

Object* Component::getOwner() const
{
	return(owner);
}