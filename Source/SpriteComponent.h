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
	Object* update() final;
	//finish functionality added to destructor

	void draw();
	void draw(EngineDefs::Vector position, EngineDefs::Float angle);
	
	EngineDefs::Vector getViewAdjustedPosition();
		
	Texture* texture{ nullptr };
	std::unique_ptr<SpriteComponent> sprite{ nullptr };
	BodyComponent* spriteBody{ nullptr };

private:
	ResourceManager * devices{ nullptr };
	
	
	
	
};

#endif