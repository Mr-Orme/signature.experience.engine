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
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Definitions.h"
#include "Vector2D.h"

class View;
class Texture;
class ResourceManager;

class GraphicsDevice
{		
public:

	//Constructors and Destructors
	GraphicsDevice(ResourceManager* devices, Uint32 width, Uint32 height, bool fullScreen);
	GraphicsDevice() =delete;
	~GraphicsDevice();

	//Rendering functions
	void Begin();
	void draw();
	void Present();

	void drawFilledCircle
	(
		Vector2D position, 
		eInt radius, 
		RGBA RGBA
	);
	bool drawBox
	(
		Vector2D topLeft, 
		Vector2D bottomRight, 
		RGBA RGBA
	);
	
	void drawOverlay
	(
		//round corner box that contains overlay
		Vector2D topLeft, 
		Vector2D bottomRight, 
		RGBA boxBackgroundColor, 
		RGBA boxBorderColor,
		//any objects drawn in stated box
		//at given position
		std::map<Texture*, Vector2D> objects			
	);

	//getters
	SDL_Renderer* getRenderer();
	//SDL_Window* getWindow();
	eInt getScreenWidth(){return SCREEN_WIDTH;}
	eInt getScreenHeight(){return SCREEN_HEIGHT;}
	View* getView();
	
	//setters
	void setView(View* view);
	bool setFont(const std::string &path, const eInt size, const RGBA color);
	
	TTF_Font* font{ nullptr };
	SDL_Color color{ 0,0,0,255 };

private:
	eFloat Center(eFloat centerOn, eFloat width);
	//Parameters
	const Uint32 SCREEN_WIDTH{ 0 };
	const Uint32 SCREEN_HEIGHT{ 0 };

	//Window(s) to display graphics
	SDL_Window* screen{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	std::unique_ptr<View> view{ nullptr };

	struct overlay
	{
		Vector2D topLeft{ 0,0 };
		Vector2D bottomRight{ 0,0 };
		RGBA boxBackgroundColor{ 0,0,0,0 };
		RGBA boxBorderColor{ 0,0,0,0 };
		std::map<Texture*, Vector2D> objects;
	};

	std::vector<overlay> overlays;

	

};

#endif