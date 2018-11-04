#ifndef SPRITE_H
#define SPRITE_H

#include <memory>

#include "Component.h"
#include "Definitions.h"

class Texture;
class SpriteComponent : public Component
{
public:
	enum class Align { Left, Right, Center, Top, Bottom, None };
	SpriteComponent(Object* owner);
	
	bool initialize(const ObjectFactoryPresets& presets) final;
	void start() final;
	Object* update() final;
	void finish() final;

	void draw();
	void draw(Position position, EngineFloat angle);
	
	Position getViewAdjustedPosition();
		
	Texture* texture{ nullptr };
	bool initialized{ false };
	std::unique_ptr<SpriteComponent> sprite{ nullptr };

private:
	ResourceManager * devices{ nullptr };
	
	
	
};

#endif