#include "StatComponent.h"
#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "AssetLibrary.h"
#include "Object.h"

StatComponent::StatComponent(Object* owner, ResourceManager* devices, eInt health):Component(owner)
{
	this->devices = devices;
	//if statistic will contain health then set input health to our current health.
	this->statistic = health;
}


//**************************************
//This "killObject" is for items that leave a sprite that is dead 
//and does not interact with the world
bool StatComponent::killObject(std::string &deathSprite)
//**************************************
{
	//Stop the physics of the object
	devices -> pDevice -> setStopPhysics(owner->getComponent<BodyComponent>());

	//grab the renderer
	SpriteComponent* sprite = owner -> getComponent<SpriteComponent>();
	//change the sprite
	
	if(sprite -> texture == devices -> assetLibrary -> getArtAsset(deathSprite))	return true;
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
eInt StatComponent::getStat()
{
	if (modifier)
		return modifier->getStat() + statistic;
	else
		return statistic;
}


//**************************************
//checks for death and deals with it
std::vector<std::unique_ptr<Object>> StatComponent::update()
//**************************************
{
	//if dead
	//Changed health to statistic, don't exactly know how this is going to work.
	if(statistic <= 0)
		//Got the conditional correct, will have to get some input on how to set Object pointer to owner's bodyComponent object.
	{
		//TODO:: update joint deletion for dying things.
		//if this is a joined object
		BodyComponent* jointToDelete = owner->getComponent<BodyComponent>();
		//modified while statement
		while(!jointToDelete->joinedWith)
		{
			//Turn off the joined object
			//BodyComponent* joined = owner->getComponent<BodyComponent>()->joinedWith.get();
		//	devices -> pDevice -> setStopPhysics(joined);
		//	//destroy the joints
//			devices->pDevice->destroyJoint(jointToDelete);
			//jointToDelete = jointToDelete->joinedWith.get();

		}
		//kill it
		killObject();
	}
	return std::vector<std::unique_ptr<Object>>();
}
//commented out finish and added destructor in its place.
