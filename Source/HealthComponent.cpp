#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "AssetLibrary.h"
#include "Object.h"

HealthComponent::HealthComponent(Object* owner):Component(owner){}

//**************************************
//gets health and resource manager from passed presets, the object starts out alive.
bool HealthComponent::initialize(const ObjectFactoryPresets& presets)
//**************************************
{
	devices = presets.devices;
	health = presets.health;
	return true;
}

//**************************************
//This "killObject" is for items that leave a sprite that is dead 
//and does not interact with the world
bool HealthComponent::killObject(std::string deathSprite)
//**************************************
{
	//Stop the physics of the object
	devices -> pDevice -> setStopPhysics(owner->getComponent<BodyComponent>());

	//grab the renderer
	SpriteComponent* sprite = owner -> getComponent<SpriteComponent>();
	//change the sprite
	//TODO: return false on bad texture!
	sprite -> texture = devices -> assetLibrary -> getArtAsset(deathSprite);	
	return true;
}
//**************************************
//For objects that disappear from the game.
bool HealthComponent::killObject()
//**************************************
{
	//this will cause it to be elimanted on the next game update
	isDead = true;
	return true;
}
void HealthComponent::start()
{
	
}

//**************************************
//checks for death and deals with it
Object* HealthComponent::update()
//**************************************
{
	//if dead
	if(health <= 0)
	{
		//TODO:: update joint deletion for dying things.
		//if this is a joined object
		//if(owner -> getJoinedWith() != nullptr)
		//{
		//	//Turn off the joined object
		//	Object* joined =  owner -> getJoinedWith();
		//	devices -> pDevice -> setStopPhysics(joined);
		//	//destroy the joints
		//	devices -> pDevice -> destroyJoint(owner);
		//}
		//kill it
		killObject();
	}
	return nullptr;
}
void HealthComponent::finish(){}
