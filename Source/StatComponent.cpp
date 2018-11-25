#include "StatComponent.h"
#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "AssetLibrary.h"
#include "Object.h"

StatComponent::StatComponent(Object* owner, ResourceManager* devices, EngineInt health):Component(owner)
{
	this->devices = devices;
<<<<<<< HEAD
	//if statistic will contain health then set input health to our current health.
	//Maybe statistic should be struct containing health?
	this->statistic = health;
=======
	this->health = health;
>>>>>>> parent of a752827... log 11/19/18
	initialized = devices && health;
}


//**************************************
//This "killObject" is for items that leave a sprite that is dead 
//and does not interact with the world
bool StatComponent::killObject(std::string deathSprite)
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
bool StatComponent::killObject()
//**************************************
{
	//this will cause it to be elimanted on the next game update
	isDead = true;
	return true;
}
EngineInt StatComponent::getStat()
{
	if (modifier)
		return modifier->getStat() + statistic;
	else
		return statistic;
}
void StatComponent::start()
{
	
}

//**************************************
//checks for death and deals with it
Object* StatComponent::update()
//**************************************
{
	//if dead
	if(health <= 0)
		//Got the conditional correct, will have to get some input on how to set Object pointer to owner's bodyComponent object.
	{
		//TODO:: update joint deletion for dying things.
		//if this is a joined object
		BodyComponent* jointToDelete = owner->getComponent<BodyComponent>();
		while(jointToDelete->joinedWith != nullptr)
		{
			//Turn off the joined object
			//BodyComponent* joined = owner->getComponent<BodyComponent>()->joinedWith.get();
		//	devices -> pDevice -> setStopPhysics(joined);
		//	//destroy the joints
			devices->pDevice->destroyJoint(jointToDelete);
			jointToDelete = jointToDelete->joinedWith.get();

		}
		//kill it
		killObject();
	}
	return nullptr;
}
void StatComponent::finish(){}
