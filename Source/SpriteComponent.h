#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "Definitions.h"
#include "tinyxml2.h"

class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(Object* owner, const SpritePresets& presets);
	SpriteComponent(Object* owner, ResourceManager* devices, tinyxml2::XMLElement* componentElement);
	//Added destructor
	~SpriteComponent();

	//deleted start, no need for functionality since constructor is called on creation
	Object* update() override;
	//finish functionality added to destructor

	void draw();
	void draw(Vector2D position, eFloat angle);
	
	Vector2D getViewAdjustedPosition();
		
	Texture* texture{ nullptr };
	std::unique_ptr<SpriteComponent> sprite{ nullptr };
	//Hack::because the sprite and corresponding body can be and different depths, we need to keep the sprite and it's body associated.
	BodyComponent* spriteBody{ nullptr };

private:
	ResourceManager * devices{ nullptr };
	
	
	
	
};

#endif