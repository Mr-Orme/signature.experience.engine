#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "Definitions.h"

class Texture;
class ObjectFactory;
class SpriteComponent : public Component
{
public:
	SpriteComponent(Object* owner);
	
	bool initialize(ObjectFactory::ObjectFactoryPresets& presets) final;
	void start() final;
	Object* update() final;
	void finish() final;

	void draw();
	void draw(Position position, EngineFloat angle);
	
	Position getViewAdjustedPosition();
		
	Texture* texture{ nullptr };
	bool initialized{ false };

private:
	ResourceManager * devices{ nullptr };
	
	
};

#endif