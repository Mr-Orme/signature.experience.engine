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
	bool loadGame(std::string levelConfigFile, std::string assetConfigFile);
	bool run();
	void update();
	void draw();

private:	

	std::unique_ptr<ResourceManager> engineDevices{ nullptr };
	std::unique_ptr<ResourceManager> gameDevices{ nullptr };
	std::unique_ptr<GraphicsDevice> mainScreen{ nullptr };
	//bool debug{ false };

	
};


#endif