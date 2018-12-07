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
	initialized = devices->pDevice->createFixture(this, presets);
	if (joinedWith)
	{
		joinedWith->start();
	}
}
//**************************************
//**************************************
//When this component is done, it destroys the body associated with the owner.
BodyComponent::~BodyComponent()
//**************************************
{
	//TODO:: this is not needed
	//if(joinedWith) joinedWith->finish(); removed joinedWith recursive wrapper from function
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

EngineFloat BodyComponent::getAngle()
{
	return devices->pDevice->getAngle(this);
}

Position BodyComponent::getPosition()
{
	return devices->pDevice->getPosition(this);
}
Position BodyComponent::getVelocity()
{
	return devices->pDevice->getVelocity(this);
}
EngineInt BodyComponent::getWidth()
{
	return owner->getComponent<SpriteComponent>()->texture->getWidth();
}
EngineInt BodyComponent::getHeight()
{
	return owner->getComponent<SpriteComponent>()->texture->getHeight();
}
void BodyComponent::setVelocity(Position velocity)
{
	this->getVelocity = velocity;
}
void BodyComponent::setXVelocity(EngineFloat xVel)
{
	this->getVelocity->x = xVel;
}
void BodyComponent::setYVelocity(EngineFloat yVel)
{
	this->getVelocity->y = yVel;
}
void BodyComponent::setAngle(EngineFloat angle)
{
	devices->pDevice->setAngle(this, angle);
}

void BodyComponent::setPosition(Position position)
{
	this->getPosition = position;
}

void BodyComponent::increaseForwardVelocity(EngineFloat num)
{
	this->getVelocity += num;
}

void BodyComponent::decreaseForwardVelocity(EngineFloat num)
{
	this->getVelocity -= num;
}

float BodyComponent::getXPos()
{
	return this->getPosition->x;
}

float BodyComponent::getYPos()
{
	return this->getPosition->y;
}

void BodyComponent::adjustAngle(EngineFloat adjustAmount)
{
	setAngle(getAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->pDevice->setLinearVelocity(this, { 0,0 });
}