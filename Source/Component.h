#ifndef COMPONENT_H
#define COMPONENT_H

#include "Definitions.h"
#include "ObjectFactory.h"

class Object;
class Component
{
public:
	Component(Object* owner);
	~Component();
	void OwnerDestroyed();
	Object* getOwner() const;

	virtual bool initialize(const ObjectFactory::ObjectFactoryPresets& presets)=0;
	virtual void start()=0;
	virtual Object* update()=0;
	virtual void finish()=0;

protected:
	Object * owner{ nullptr };
};


#endif