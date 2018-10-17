/*
* Author:	Sean Orme
*			UALR
* 
* Date:		May, 2015	
*
* File:		GraphicsDevice.h
*
* Purpose:	Declaration of the SDL GraphicsDevice class
*/

#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <memory>
#include <vector>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Definitions.h"
#include "View.h"
class Texture;


class GraphicsDevice
{
		
public:

	//Constructors and Destructors
	GraphicsDevice(Uint32, Uint32);

	//startup and Shutdown
	bool initialize(bool);
	bool ShutDown();

	//Rendering functions
	void Begin();
	void draw();
	void Present();

	void Text2Screen(std::string text, Position position);
	void Text2Screen(std::string text, EngineFloat x, EngineFloat y);
	void Notice2Screen(std::string text);

	void drawFilledCircle(Position position, EngineInt radius, RGBA RGBA);
	bool drawBox(Position topLeft, Position bottomRight, RGBA RGBA);
	
	void drawOverlay
		(
			//round corner box that contains overlay
			Position topLeft, Position bottomRight, RGBA boxBackgroundColor, RGBA boxBorderColor, 
			//any objects drawn in stated box
			//at given position
			std::map<Texture*, Position> objects
			
		);

	//getters
	SDL_Renderer* getRenderer();
	//SDL_Window* getWindow();
	EngineInt getScreenWidth(){return SCREEN_WIDTH;}
	EngineInt getScreenHeight(){return SCREEN_HEIGHT;}
	View* getView();
	
	//setters
	void setView(View* view);
	bool setFont(std::string path, EngineInt size, RGBA color);

private:
	EngineFloat Center(EngineFloat centerOn, EngineFloat width);
	//Parameters
	const Uint32 SCREEN_WIDTH{ 0 };
	const Uint32 SCREEN_HEIGHT{ 0 };

	//Window(s) to display graphics
	SDL_Window* screen{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	std::unique_ptr<View> view{ nullptr };

	struct overlay
	{
		Position topLeft{ 0 };
		Position bottomRight{ 0 };
		RGBA boxBackgroundColor{ 0,0,0,0 };
		RGBA boxBorderColor{ 0,0,0,0 };
		std::map<Texture*, Position> objects;
	};

	std::vector<overlay> overlays;

	TTF_Font* font;
	RGBA color;

};

#endif