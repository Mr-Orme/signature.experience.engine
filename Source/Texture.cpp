#include <iostream>
#include "Texture.h"
#include "GraphicsDevice.h"

using namespace std;


Texture::~Texture()
{
	free();
}


Texture::Texture(GraphicsDevice* gDevice, std::string path, std::string text)
{

	//Destroy existing texture information
	free();

	if (path != " ")
	{
		//Load the image
		SDL_Surface* surface = IMG_Load(path.c_str());

		//If the image loaded
		if (surface == nullptr)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());

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
	else if (text != " ")
	{
		this->text = SDL_CreateTextureFromSurface(
			gDevice->getRenderer(),
			TTF_RenderText_Solid(
				gDevice->font,
				path.c_str(),
				gDevice->color));
		initialzied = this->text != nullptr;
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
	if (text != nullptr)
	{
		SDL_DestroyTexture(text);
		text = nullptr;
		vertical = Align::None;
		horizontal = Align::None;
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
	if (text)
	{
		int textWidth, textHeight;
		Position textPosition{ position.x, position.y };
		SDL_QueryTexture(text, NULL, NULL, &textWidth, &textHeight);
		switch (vertical)
		{
		case Align::Top:
			break;
		case Align::Bottom:
			textPosition.y = position.y + height - textHeight;
			break;
		case Align::Center:
			textPosition.y = position.y + (height - textHeight)/2;
			break;
		}
		switch (vertical)
		{
		case Align::Left:
			break;
		case Align::Right:
			textPosition.x = position.x + width - textWidth;
			break;
		case Align::Center:
			textPosition.x = position.x + (width - textWidth)/2;
			break;
		}
		SDL_Rect renderQuad = { (int)textPosition.x, (int)textPosition.y, textWidth, textHeight };

		//Render to screen
		SDL_RenderCopyEx(renderer, text, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);

	}

}