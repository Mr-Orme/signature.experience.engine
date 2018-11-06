#include<iostream>
#include "SDL2_gfxPrimitives.h"
#include "SDL_mixer.h"
#include "GraphicsDevice.h"
#include "View.h"
#include "Texture.h"
#include "SoundDevice.h"




GraphicsDevice::GraphicsDevice(Uint32 width, Uint32 height) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height)
{

}


bool GraphicsDevice::initialize(bool fullScreen)
{


	//initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return(false);
	}

	//initialize SDL_image subsystems
	if(!IMG_Init(IMG_INIT_PNG))
	{
		printf( "SDL_image could not initialize! SDL_Error: %s\n", IMG_GetError() );
		return(false);
	}

	//initialize SDL_ttf subsystems
	if(TTF_Init()==-1)
	{
		printf( "SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError() );
		return(false);
	}

	if(!fullScreen)
	{
		//Construct and check window construction
		screen = SDL_CreateWindow("Signature Game Engine",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		screen = SDL_CreateWindow("Signature Game Engine",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	}
	if(screen==nullptr)
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return(false);
	}

	//Construct the renderer
	renderer = SDL_CreateRenderer(screen,-1,SDL_RENDERER_ACCELERATED);
	if(renderer==nullptr)
	{
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return(false);
	}

	//set the background color (default)
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	//========================================
	//create view
	//========================================
	view = std::make_unique<View>();
	view->initialize({ 0, 0 });

	return(true);

}

bool GraphicsDevice::ShutDown()
{
	//Free the window
	SDL_DestroyWindow(screen);
	screen = nullptr;

	//Free renderer
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	

	//Quit SDL Subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();


	return(true);
}




void GraphicsDevice::Begin()
{
	SDL_RenderClear(renderer);
}

void GraphicsDevice::draw()
{
	//********************************draw overlays*************************************
	int cornerRadius = 3; //radius of curve of message box corners.
	//the text vector holds any messages.
	while(!overlays.empty())
	{

		overlay currOverlay = overlays.back();
		overlays.pop_back();

		
		//draw the box
		roundedBoxRGBA
		(
			renderer, 
			(Sint16)(currOverlay.topLeft.x),
			(Sint16)(currOverlay.topLeft.y),
			(Sint16)(currOverlay.bottomRight.x),
			(Sint16)(currOverlay.bottomRight.y),
			(Sint16)(cornerRadius),
			(Uint8)(currOverlay.boxBackgroundColor.R),
			(Uint8)(currOverlay.boxBackgroundColor.G),
			(Uint8)(currOverlay.boxBackgroundColor.B),
			(Uint8)(currOverlay.boxBackgroundColor.A)
		);
		//draw the border
		roundedRectangleRGBA
		(
			renderer, 
			(Sint16)(currOverlay.topLeft.x),
			(Sint16)(currOverlay.topLeft.y),
			(Sint16)(currOverlay.bottomRight.x),
			(Sint16)(currOverlay.bottomRight.y),
			(Sint16)(cornerRadius),
			(Uint8)(currOverlay.boxBorderColor.R),
			(Uint8)(currOverlay.boxBorderColor.G),
			(Uint8)(currOverlay.boxBorderColor.B),
			(Uint8)(currOverlay.boxBorderColor.A)
		);

		//draw the objects
		for( auto object : currOverlay.objects)
		{
			object.first ->draw(renderer, object.second, 0, NULL);

		}

		
			
//!!!!!!!!!!!!!!!!!!!!!!!!!!this needs to be elsewhere!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	EngineInt spheresFound = 0; //number of spheres found
//	//if we found all the spheres.
//	if(spheresFound >= 6)
//	{
//		ResourceManager* devices = levelExit -> getComponent<BodyComponent>() -> getDevices().get();
//		//stop the physics on the trapdoor so we can walk onto that square.
//		devices -> pDevice -> setStopPhysics(levelExit.get());
//		//get rid of the notice stating we need to find the spheres.
//		Notice notice = {15, 0, W, ""};
//		devices -> getNoticesLibrary() -> RemoveAsset(notice);
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	}
	//back to black. . .
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	
}

void GraphicsDevice::Present()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* GraphicsDevice::getRenderer()
{
	return(renderer);
}

View * GraphicsDevice::getView()
{
	return view.get();
}

void GraphicsDevice::setView(View * view)
{
	this->view = std::unique_ptr<View>(view);
}


bool GraphicsDevice::setFont(std::string path, EngineInt size, RGBA color)
{
	font = TTF_OpenFont(path.c_str(), size);
	if( font == nullptr )
	{
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	this->color =
	{
		(Uint8)(color.R),
		(Uint8)(color.G),
		(Uint8)(color.B),
		(Uint8)(color.A)
	};
	
	return(true);

}



////**************************************
////reverses the order of sprites so that the player is on top.
//void GraphicsDevice::ReverseOrder()
////**************************************
//{
//	std::reverse(sprites.begin(), sprites.end());
//}
//**************************************
//draws a filled circle.
void GraphicsDevice::drawFilledCircle(Position position, EngineInt radius, RGBA RGBA)
//**************************************
{
	filledCircleRGBA
	(
		renderer, 
		(Sint16)(position.x),
		(Sint16)(position.y),
		(Sint16)(radius),
		(Uint8)(RGBA.R),
		(Uint8)(RGBA.G),
		(Uint8)(RGBA.B),
		(Uint8)(RGBA.A)
	);
}

bool GraphicsDevice::drawBox(Position topLeft, Position bottomRight, RGBA RGBA)
{
	boxRGBA
	(
		renderer,
		(Sint16)(topLeft.x),
		(Sint16)(topLeft.y),
		(Sint16)(bottomRight.x),
		(Sint16)(bottomRight.y),
		(Uint8)(RGBA.R),
		(Uint8)(RGBA.G),
		(Uint8)(RGBA.B),
		(Uint8)(RGBA.A)
	);
	return true;
}

void GraphicsDevice::drawOverlay(Position topLeft, Position bottomRight, RGBA boxBackgroundColor, RGBA boxBorderColor, std::map<Texture*, Position> objects)
{
	overlay newOverlay = {topLeft, bottomRight, boxBackgroundColor, boxBorderColor, objects};
	overlays.push_back(newOverlay);
}

EngineFloat GraphicsDevice::Center(EngineFloat centerOn, EngineFloat width)
{
			
			
	EngineFloat point = (centerOn - width)/2;
			
	return point;
}