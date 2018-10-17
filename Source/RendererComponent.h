#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"
#include "Definitions.h"

class Texture;
class ObjectFactory;
class RendererComponent : public Component
{
public:
	RendererComponent(Object* owner);
	
	bool initialize(ObjectFactory::ObjectFactoryPresets& presets) final;
	void start() final;
	Object* update() final;
	void finish() final;

	void draw();
	void draw(Position position, ENGINE_FLT angle);
	
	Position getViewAdjustedPosition();
		
	Texture* texture{ nullptr };
	bool initialized{ false };

private:
	ResourceManager * devices{ nullptr };
	
	
};

#endif