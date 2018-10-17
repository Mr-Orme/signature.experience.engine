#include "RendererComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "AssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "GraphicsDevice.h"

RendererComponent::RendererComponent(Object* owner):Component(owner){}


//**************************************
//on the first pass, we set up the texture for the object
bool RendererComponent::initialize(ObjectFactory::ObjectFactoryPresets& presets)
//**************************************
{
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
void RendererComponent::draw()
//**************************************
{
	Position updatedPosition;	

	//adjust position.
	updatedPosition = getViewAdjustedPosition();

	auto angle = devices -> pDevice -> getAngle(owner);

	//draw sprite.
	draw(updatedPosition, angle);
}

void RendererComponent::draw(Position position, EngineFloat angle)
{
	texture ->draw(devices -> gDevice -> getRenderer(), position, angle, NULL);
}

void RendererComponent::start(){}

Object* RendererComponent::update(){return nullptr;}

//**************************************
//adjusts the position based on the view.
Position RendererComponent::getViewAdjustedPosition()
//**************************************
{	
	//adjust position.
	return (devices->pDevice->getPosition(owner) 
		+ devices->gDevice->getView()->position);
}

void RendererComponent::finish(){}