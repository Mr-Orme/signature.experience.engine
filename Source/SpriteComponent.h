#ifndef SPRITE_H
#define SPRITE_H

#include <memory>

#include "Component.h"
#include "Definitions.h"

class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(Object* owner, const SpritePresets& presets);
	//Added destructor
	~SpriteComponent();

	//deleted start, no need for functionality since constructor is called on creation
	Object* update() final;
	//finish functionality added to destructor

	void draw();
	void draw(Position position, EngineFloat angle);
	
	Position getViewAdjustedPosition();
		
	Texture* texture{ nullptr };
	std::unique_ptr<SpriteComponent> sprite{ nullptr };
	BodyComponent* spriteBody{ nullptr };

private:
	ResourceManager * devices{ nullptr };
	
	
	
	
};

#endif