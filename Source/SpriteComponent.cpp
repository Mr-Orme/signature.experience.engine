#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "AssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "GraphicsDevice.h"
#include "Object.h"
#include "Initializers.h"

SpriteComponent::SpriteComponent(Object* owner, const SpritePresets& presets):Component(owner)
{
	//Took conditional statement from start function and put into constructor. If there is a sprite continue with constructor.
	if (sprite)
	{

		this->devices = presets.Devices;
		this->texture = presets.spriteTexture;
		initialized = (devices && texture);
	}
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


Object* SpriteComponent::update(){return nullptr;}

//**************************************
//adjusts the position based on the view.
Position SpriteComponent::getViewAdjustedPosition()
//**************************************
{	
	//adjust position.
	return (devices->pDevice->getPosition(spriteBody) 
		+ devices->gDevice->getView()->position);
}
//No need for finish function since our destructor will handle this process
/*void SpriteComponent::finish()
{
	if (sprite)
	{
		sprite->finish();
	}
}*/