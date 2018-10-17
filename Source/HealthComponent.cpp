#include "HealthComponent.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "AssetLibrary.h"
#include "Object.h"

HealthComponent::HealthComponent(Object* owner):Component(owner){}
HealthComponent::~HealthComponent(){}

//**************************************
//gets health and resource manager from passed presets, the object starts out alive.
bool HealthComponent::initialize(ObjectFactory::ObjectFactoryPresets& presets)
//**************************************
{
	devices = presets.devices;
	health = devices -> getAssetLibrary() -> getObjectStats(presets.objectType).health;
	return true;
}

//**************************************
//This "killObject" is for items that leave a sprite that is dead 
//and does not interact with the world
bool HealthComponent::killObject(std::string deathSprite)
//**************************************
{
	//Stop the physics of the object
	devices -> getPhysicsDevice() -> setStopPhysics(owner);

	//grab the renderer
	RendererComponent* compRenderer = owner -> getComponent<RendererComponent>();
	//change the sprite
	//TODO: return false on bad texture!
	compRenderer -> texture = devices -> getAssetLibrary() -> getArtAsset(deathSprite);	
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
		//if this is a joined object
		if(owner -> getJoinedWith() != nullptr)
		{
			//Turn off the joined object
			Object* joined =  owner -> getJoinedWith();
			devices -> getPhysicsDevice() -> setStopPhysics(joined);
			//destroy the joints
			devices -> getPhysicsDevice() -> destroyJoint(owner);
		}
		//kill it
		killObject();
	}
	return nullptr;
}
void HealthComponent::finish(){}
