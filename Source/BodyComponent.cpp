#include "BodyComponent.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "AssetLibrary.h"
#include "Object.h"
#include "PhysicsDevice.h"
#include "Texture.h"
#include "ICallBack.h"
#include "BodyCallBack.h"
#include "Vector2D.h"



BodyComponent::BodyComponent(Object* owner, ResourceManager* devices, BodyPresets presets)
	:Component(owner), devices(devices)
{
	for (auto& callBack : presets.callBacks)
	{
		callBacks.push_back(std::unique_ptr<BodyCallBack>(static_cast<BodyCallBack*>(callBack.release())));		
	}
	presets.callBacks.clear();
	
	//Create fixture.
	devices->pDevice->createFixture(this, std::move(presets));

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

eFloat BodyComponent::getAngle()
{
	return devices->pDevice->getAngle(this);
}

Vector2D BodyComponent::getPosition()
{
	return devices->pDevice->getPosition(this);
}
Vector2D BodyComponent::getVelocity()
{
	return devices->pDevice->getVelocity(this);
}
eInt BodyComponent::getWidth()
{
	return owner->getComponent<SpriteComponent>()->texture->width;
}
eInt BodyComponent::getHeight()
{
	return owner->getComponent<SpriteComponent>()->texture->height;
}
void BodyComponent::setVelocity(Vector2D velocity)
{
	devices->pDevice->setLinearVelocity(this, velocity);
}
void BodyComponent::setXVelocity(eFloat value)
{
	devices->pDevice->setLinearVelocity
	(
		this, 
		{ value, devices->pDevice->getLinearVelocity(this).y }
	);
}
void BodyComponent::setYVelocity(eFloat value)
{
	devices->pDevice->setLinearVelocity
	(
		this,
		{ devices->pDevice->getLinearVelocity(this).x, value }
	);
}
void BodyComponent::setAngle(eFloat angle)
{
	devices->pDevice->setAngle(this, angle);
}

void BodyComponent::setPosition(Vector2D position)
{
	devices->pDevice->setTransform(this, position, devices->pDevice->getAngle(this));
}

void BodyComponent::accelerate(eFloat force)
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
void BodyComponent::deccelerate(eFloat force)
{
	accelerate(force*-1);
}

eFloat BodyComponent::getXPos()
{
	return getPosition().x;
}

eFloat BodyComponent::getYPos()
{
	return getPosition().y;
}

void BodyComponent::rotate(eFloat degrees)
{
	setAngle(getAngle() + degrees);
}

void BodyComponent::linearStop()
{
	devices->pDevice->setLinearVelocity(this, { 0,0 });
}