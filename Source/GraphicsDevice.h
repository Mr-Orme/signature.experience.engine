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

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Definitions.h"

class View;
class Texture;

class GraphicsDevice
{		
public:

	//Constructors and Destructors
	GraphicsDevice(Uint32, Uint32, bool fullScreen);
	GraphicsDevice() =delete;
	~GraphicsDevice();

	//startup and Shutdown
	//Going to have to get creative here. Shutdown sent to Destructor but initialize is a little bit trickier.
	bool initialize(bool);

	//Rendering functions
	void Begin();
	void draw();
	void Present();

	void drawFilledCircle
	(
		EngineDefs::Vector position, 
		EngineDefs::Int radius, 
		RGBA RGBA
	);
	bool drawBox
	(
		EngineDefs::Vector topLeft, 
		EngineDefs::Vector bottomRight, 
		RGBA RGBA
	);
	
	void drawOverlay
	(
		//round corner box that contains overlay
		EngineDefs::Vector topLeft, 
		EngineDefs::Vector bottomRight, 
		RGBA boxBackgroundColor, 
		RGBA boxBorderColor,
		//any objects drawn in stated box
		//at given position
		std::map<Texture*, EngineDefs::Vector> objects			
	);

	//getters
	SDL_Renderer* getRenderer();
	//SDL_Window* getWindow();
	EngineDefs::Int getScreenWidth(){return SCREEN_WIDTH;}
	EngineDefs::Int getScreenHeight(){return SCREEN_HEIGHT;}
	View* getView();
	
	//setters
	void setView(View* view);
	bool setFont(const std::string &path, const EngineDefs::Int size, const RGBA color);
	
	TTF_Font* font{ nullptr };
	SDL_Color color{ 0,0,0,255 };

private:
	EngineDefs::Float Center(EngineDefs::Float centerOn, EngineDefs::Float width);
	//Parameters
	const Uint32 SCREEN_WIDTH{ 0 };
	const Uint32 SCREEN_HEIGHT{ 0 };

	//Window(s) to display graphics
	SDL_Window* screen{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	std::unique_ptr<View> view{ nullptr };

	struct overlay
	{
		EngineDefs::Vector topLeft{ 0 };
		EngineDefs::Vector bottomRight{ 0 };
		RGBA boxBackgroundColor{ 0,0,0,0 };
		RGBA boxBorderColor{ 0,0,0,0 };
		std::map<Texture*, EngineDefs::Vector> objects;
	};

	std::vector<overlay> overlays;

	

};

#endif