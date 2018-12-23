#include "BodyComponent.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "AssetLibrary.h"
#include "Object.h"
#include "PhysicsDevice.h"
#include "Texture.h"



BodyComponent::BodyComponent(Object* owner, ResourceManager* devices, BodyPresets presets):Component(owner), devices(devices)
{
	//Create fixture.
	//initialized =
	devices->pDevice->createFixture(this, presets);
	//if (joinedWith)
	//{
	//	joinedWith->start();
	//}
}
//**************************************
//**************************************
//When this component is done, it destroys the body associated with the owner.
BodyComponent::~BodyComponent()
//**************************************
{

	//remove the physics body
	if (!devices->pDevice->removeObject(this))
	{
		printf("Object could not be removed from Physics World");
		exit(1);
	}
}


//**************************************
//
Object* BodyComponent::update()
//**************************************
{
	if (joinedWith)
	{
		if (Object* returnedObject = joinedWith->update(); returnedObject)
		{
			return returnedObject;
		}
	}
	return nullptr;
}

EngineDefs::Float BodyComponent::getAngle()
{
	return devices->pDevice->getAngle(this);
}

EngineDefs::Vector BodyComponent::getPosition()
{
	return devices->pDevice->getPosition(this);
}
EngineDefs::Vector BodyComponent::getVelocity()
{
	return devices->pDevice->getVelocity(this);
}
EngineDefs::Int BodyComponent::getWidth()
{
	return owner->getComponent<SpriteComponent>()->texture->getWidth();
}
EngineDefs::Int BodyComponent::getHeight()
{
	return owner->getComponent<SpriteComponent>()->texture->getHeight();
}
void BodyComponent::setVelocity(EngineDefs::Vector velocity)
{
	devices->pDevice->setLinearVelocity(this, velocity);
}
void BodyComponent::setXVelocity(EngineDefs::Float value)
{
	devices->pDevice->setLinearVelocity
	(
		this, 
		{ value, devices->pDevice->getLinearVelocity(this).y }
	);
}
void BodyComponent::setYVelocity(EngineDefs::Float value)
{
	devices->pDevice->setLinearVelocity
	(
		this,
		{ devices->pDevice->getLinearVelocity(this).x, value }
	);
}
void BodyComponent::setAngle(EngineDefs::Float angle)
{
	devices->pDevice->setAngle(this, angle);
}

void BodyComponent::setPosition(EngineDefs::Vector position)
{
	devices->pDevice->setTransform(this, position, devices->pDevice->getAngle(this));
}

void BodyComponent::accelerate(EngineDefs::Float force)
{
	devices->pDevice->accelerate
	(
		this,
		{
			force*cos(getAngle() * PI / 180.0f - PI / 2),
			force*sin(getAngle() * PI / 180.0f - PI / 2)
		}
	);
}
void BodyComponent::deccelerate(EngineDefs::Float force)
{
	accelerate(force*-1);
}

EngineDefs::Float BodyComponent::getXPos()
{
	return getPosition().x;
}

EngineDefs::Float BodyComponent::getYPos()
{
	return getPosition().y;
}

void BodyComponent::rotate(EngineDefs::Float degrees)
{
	setAngle(getAngle() + degrees);
}

void BodyComponent::linearStop()
{
	devices->pDevice->setLinearVelocity(this, { 0,0 });
}