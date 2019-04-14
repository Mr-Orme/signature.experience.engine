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



BodyComponent::BodyComponent(Object* owner, ResourceManager* devices, BodyPresets & presets)
	:Component(owner), devices(devices)
{
	/*for (auto& callBack : presets.callBacks)
	{
		callBacks.push_back(std::unique_ptr<BodyCallBack>(static_cast<BodyCallBack*>(callBack.release())));		
	}
	presets.callBacks.clear();*/
	callBacks = std::move(presets.callBacks);
	
	//Create fixture.
	devices->pDevice->createFixture(this, presets);

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
std::vector<std::unique_ptr<Object>> BodyComponent::update()
//**************************************
{
	if (joinedWith)
	{
		auto returnedObject = joinedWith->update(); 
		
			return returnedObject;
	}
	return std::vector<std::unique_ptr<Object>>();
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
void BodyComponent::setAngle(Vector2D angle)
{
	
	devices->pDevice->setAngle(this, angle.getAngleDegrees({ 0,1 }));
}

void BodyComponent::setPosition(Vector2D position)
{
	devices->pDevice->setTransform(this, position, devices->pDevice->getAngle(this));
}

void BodyComponent::accelerate(Vector2D force)
{
	force.Normalize();
	devices->pDevice->accelerate(this, force);
}
void BodyComponent::deccelerate(Vector2D force)
{
	force.Normalize();
	accelerate(force*-1);
}


//void BodyComponent::applyRotationalForce(Vector2D degrees)
//{
//
//	setAngle(getAngle() + degrees);
//}

void BodyComponent::linearStop(Vector2D notUsed)
{
	devices->pDevice->setLinearVelocity(this, { 0,0 });
}