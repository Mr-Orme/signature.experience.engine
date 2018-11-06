#include <iostream>
#include "Texture.h"
#include "GraphicsDevice.h"

using namespace std;


Texture::~Texture()
{
	free();
}


Texture::Texture(GraphicsDevice* gDevice, std::string pathOrText, bool isSprite)
{

	//Destroy existing texture information
	free();

	if (isSprite)
	{
		//Load the image
		SDL_Surface* surface = IMG_Load(pathOrText.c_str());

		//If the image loaded
		if (surface == nullptr)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", pathOrText.c_str(), IMG_GetError());

		}
		else
		{

			//Color key image
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));

			//Create an optimized image
			sprite = SDL_CreateTextureFromSurface(gDevice->getRenderer(), surface);
			initialzied = sprite != nullptr;
			//Free the loaded image; no longer needed
			SDL_FreeSurface(surface);
		}
	}
	else 
	{
		this->sprite = SDL_CreateTextureFromSurface(
			gDevice->getRenderer(),
			TTF_RenderText_Solid(
				gDevice->font,
				pathOrText.c_str(),
				gDevice->color));
		initialzied = this->sprite != nullptr;
	}

		//set the height and width from the texture
		SDL_QueryTexture(sprite, NULL, NULL, &width, &height);

	
}

bool Texture::load(SDL_Texture* texture)
{
	this -> sprite = texture;
	//set the height and width from the texture
	SDL_QueryTexture(texture,NULL,NULL,&width,&height);
	return true;
}

void Texture::free()
{
	if(sprite != nullptr)
	{
		SDL_DestroyTexture(sprite);
  		sprite = nullptr;
		width = 0;
		height = 0;
	}

}
	   

void Texture::draw(SDL_Renderer* renderer, Position position, EngineFloat angle, SDL_Rect* clip)
{
	if (sprite)
	{
		//set rendering space and render to screen
		SDL_Rect renderQuad = { (int)position.x, (int)position.y, width, height };

		//set clip rendering dimensions
		if (clip != nullptr) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopyEx(renderer, sprite, clip, &renderQuad, angle, NULL, SDL_FLIP_NONE);
	}
	

}