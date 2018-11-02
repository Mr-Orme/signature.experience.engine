#include "BodyComponent.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "AssetLibrary.h"
#include "Object.h"
#include "PhysicsDevice.h"
#include "Texture.h"

BodyComponent::BodyComponent(Object* owner):Component(owner){}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::initialize(ObjectFactory::ObjectFactoryPresets& presets)
//**************************************
{	
	SpriteComponent* compRenderer = owner -> getComponent<SpriteComponent>();
	PhysicsDevice::PhysicsStats physics;
	if(compRenderer != nullptr)
	{
		//store the resource manager.
		devices = presets.devices;
				
		//get physics based on object type.
		physics = devices -> assetLibrary -> getObjectPhysics(presets.objectType);
				
		//Create fixture.
		devices -> pDevice -> createFixture
			(
			owner,
			physics,
			presets
			);
	}	
	return true;
}

void BodyComponent::start(){}

//**************************************
//
Object* BodyComponent::update()
//**************************************
{
	return nullptr;
}
//**************************************
//**************************************
//When this component is done, it destroys the body associated with the owner.
void BodyComponent::finish()
//**************************************
{
	//remove the physics body
	if(!devices -> pDevice -> removeObject(owner))
	{
		printf( "Object could not be removed from Physics World");
		exit(1);					
	}
}
EngineFloat BodyComponent::getAngle()
{
	return devices->pDevice->getAngle(owner);
}

Position BodyComponent::getPosition()
{
	return devices->pDevice->getPosition(owner);
}
Position BodyComponent::getVelocity()
{
	return devices->pDevice->getVelocity(owner);
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
	devices->pDevice->setAngle(owner, angle);
}

void BodyComponent::adjustAngle(EngineFloat adjustAmount)
{
	setAngle(getAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->pDevice->setLinearVelocity(owner, { 0,0 });
}