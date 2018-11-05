#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "AssetLibrary.h"
#include "Object.h"

HealthComponent::HealthComponent(Object* owner, ResourceManager* devices, EngineInt health):Component(owner)
{
	this->devices = devices;
	this->health = health;
	initialized = devices && health;
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
	
	if(sprite -> texture = devices -> assetLibrary -> getArtAsset(deathSprite))	return true;
	else return false;
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
