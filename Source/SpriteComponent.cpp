#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "AssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "GraphicsDevice.h"
#include "Object.h"

SpriteComponent::SpriteComponent(Object* owner):Component(owner){}

//**************************************
//on the first pass, we set up the texture for the object
bool SpriteComponent::initialize(const ObjectFactoryPresets& presets)
//**************************************
{
	//MrOrme:: need to add joint functionality
	//this will get hit twice, so we only want it done once.
	if(!initialized)
	{
		devices = presets.devices;
		//grab the sprite from the library.
		texture = presets.devices -> assetLibrary -> getArtAsset(presets.objectType);
		initialized = texture != nullptr;
	}
	return initialized;
}
//**************************************
// updates the position based on the view and draws the sprite
void SpriteComponent::draw()
//**************************************
{
	Position updatedPosition;	

	//adjust position.
	updatedPosition = getViewAdjustedPosition();

	auto angle = devices -> pDevice -> getAngle(owner->getComponent<BodyComponent>());

	//draw sprite.
	draw(updatedPosition, angle);
	if (sprite)
	{
		sprite->draw();
	}
}

void SpriteComponent::draw(Position position, EngineFloat angle)
{
	texture ->draw(devices -> gDevice -> getRenderer(), position, angle, NULL);
}

void SpriteComponent::start()
{
	if (sprite)
	{
		sprite->start();
	}
}

Object* SpriteComponent::update(){return nullptr;}

//**************************************
//adjusts the position based on the view.
Position SpriteComponent::getViewAdjustedPosition()
//**************************************
{	
	//adjust position.
	return (devices->pDevice->getPosition(owner->getComponent<BodyComponent>()) 
		+ devices->gDevice->getView()->position);
}

void SpriteComponent::finish()
{
	if (sprite)
	{
		sprite->finish();
	}
}