#include "BodyComponent.h"
#include "RendererComponent.h"
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
	RendererComponent* compRenderer = owner -> getComponent<RendererComponent>();
	PhysicsDevice::PhysicsStats physics;
	if(compRenderer != nullptr)
	{
		//store the resource manager.
		devices = presets.devices;
				
		//get physics based on object type.
		physics = devices -> getAssetLibrary() -> getObjectPhysics(presets.objectType);
				
		//Create fixture.
		devices -> getPhysicsDevice() -> createFixture
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
	if(!devices -> getPhysicsDevice() -> removeObject(owner))
	{
		printf( "Object could not be removed from Physics World");
		exit(1);					
	}
}
ENGINE_FLT BodyComponent::getAngle()
{
	return devices->getPhysicsDevice()->getAngle(owner);
}

Position BodyComponent::getPosition()
{
	return devices->getPhysicsDevice()->getPosition(owner);
}
Position BodyComponent::getVelocity()
{
	return devices->getPhysicsDevice()->getVelocity(owner);
}
EngineInt BodyComponent::getWidth()
{
	return owner->getComponent<RendererComponent>()->texture->getWidth();
}
EngineInt BodyComponent::getHeight()
{
	return owner->getComponent<RendererComponent>()->texture->getHeight();
}
void BodyComponent::setAngle(ENGINE_FLT angle)
{
	devices->getPhysicsDevice()->setAngle(owner, angle);
}

void BodyComponent::adjustAngle(ENGINE_FLT adjustAmount)
{
	setAngle(getAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->getPhysicsDevice()->setLinearVelocity(owner, { 0,0 });
}