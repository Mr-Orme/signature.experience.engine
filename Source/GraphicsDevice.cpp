#include "SDL2_gfxPrimitives.h"
#include "SDL_mixer.h"
#include "GraphicsDevice.h"
#include "View.h"
#include "Texture.h"
//#include "SoundDevice.h"
#include "ResourceManager.h"




GraphicsDevice::GraphicsDevice(ResourceManager* devices, Uint32 width, Uint32 height, bool fullScreen = true) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height)
{
	//initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	//initialize SDL_image subsystems
	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_Error: %s\n", IMG_GetError());
	}

	//initialize SDL_ttf subsystems
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
	}

	if (!fullScreen)
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
	if (screen == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	//Construct the renderer
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	//set the background color (default)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//========================================
	//create view
	//========================================
	Vector2D position{ 0,0 };
	view = std::make_unique<View>(position, devices);
}


GraphicsDevice::~GraphicsDevice()
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


bool GraphicsDevice::setFont(const std::string &path, const eInt size, const RGBA color)
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
void GraphicsDevice::drawFilledCircle(Vector2D position, eInt radius, RGBA RGBA)
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

bool GraphicsDevice::drawBox(Vector2D topLeft, Vector2D bottomRight, RGBA RGBA)
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

void GraphicsDevice::drawOverlay(Vector2D topLeft, Vector2D bottomRight, RGBA boxBackgroundColor, RGBA boxBorderColor, std::map<Texture*, Vector2D> objects)
{
	overlay newOverlay = {topLeft, bottomRight, boxBackgroundColor, boxBorderColor, objects};
	overlays.push_back(newOverlay);
}

eFloat GraphicsDevice::Center(eFloat centerOn, eFloat width)
{
			
			
	eFloat point = (centerOn - width)/2;
			
	return point;
}