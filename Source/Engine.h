#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <memory>

#include "Definitions.h"
#include "Object.h"
#include "ResourceManager.h"

class Engine{
public:	
	~Engine();
	bool initialize(std::string engineFile);
	void reset();
	bool loadLevel(std::string levelConfigFile, std::string assetConfigFile);
	bool run();
	void update();
	void draw();

private:
	std::vector<std::unique_ptr<Object>> objects;
	std::vector<std::unique_ptr<Object>> newObjects;

	std::unique_ptr<ResourceManager> devices{ nullptr };
	EngineFloat gameTime{ 0.0 };
	//bool debug{ false };
	EngineInt SCREEN_WIDTH{ 0 };
	EngineInt SCREEN_HEIGHT{ 0 };
	
};


#endif