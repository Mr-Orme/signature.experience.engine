#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "Definitions.h"
class GraphicsDevice;
class Texture{

    public:
		
		Texture() = delete;
		Texture(GraphicsDevice* gDevice, const std::string & pathOrText, const bool isSprite = true);
        ~Texture();

        void draw(SDL_Renderer* renderer, Vector2D position, eFloat angle, SDL_Rect* clip = NULL );
		
		int width{ 0 };
		int height{ 0 };
		bool initialzied{ false };
private:
        //The actual hardware texture. created and destroyed by SDL
		SDL_Texture * sprite{ nullptr };


};

#endif