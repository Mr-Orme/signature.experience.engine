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
}

void BodyComponent::start()
{
	if (joinedWith)
	{
		joinedWith->start();
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
//**************************************
//**************************************
//When this component is done, it destroys the body associated with the owner.
void BodyComponent::finish()
//**************************************
{
	if(joinedWith) joinedWith->finish();
	//remove the physics body
	if(!devices -> pDevice -> removeObject(this))
	{
		printf( "Object could not be removed from Physics World");
		exit(1);					
	}
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
void BodyComponent::setAngle(EngineFloat angle)
{
	devices->pDevice->setAngle(this, angle);
}

void BodyComponent::adjustAngle(EngineFloat adjustAmount)
{
	setAngle(getAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->pDevice->setLinearVelocity(this, { 0,0 });
}