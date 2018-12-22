#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "Definitions.h"
class GraphicsDevice;
class Texture{

    public:
		
		//Loads image at specified path
		//passing string value by reference
		Texture(GraphicsDevice* gDevice, const std::string pathOrText, const bool isSprite = true);

		//Deallocates memory
        ~Texture();

        
		bool load(SDL_Texture* texture);

        //Deallocates texture
        void free();

        //Renders texture at given point
		
		void draw(SDL_Renderer* renderer, EngineDefs::Vector position, EngineDefs::Float angle, SDL_Rect* clip = NULL );
		int getWidth() { return width; }
		int getHeight() { return height; }
		//Image dimensions
		
		bool initialzied{ false };
    private:
		int width{ 0 };
		int height{ 0 };
        //The actual hardware texture
		SDL_Texture * sprite{ nullptr };


};

#endif