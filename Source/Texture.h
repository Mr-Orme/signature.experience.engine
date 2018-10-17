#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "Definitions.h"

class Texture{

    public:
        //initializes variables
		Texture() {};

        //Deallocates memory
        ~Texture();

        //Loads image at specified path
        bool load(SDL_Renderer* renderer, std::string path );
		bool load(SDL_Texture* texture);

        //Deallocates texture
        void free();

        //Renders texture at given point
		
		void draw(SDL_Renderer* renderer, Position position, EngineFloat angle, SDL_Rect* clip = NULL );

        //gets image dimensions
        int getWidth();
        int getHeight();

    private:

        //The actual hardware texture
		SDL_Texture * texture{ nullptr };

        //Image dimensions
		int width{ 0 };
		int height{ 0 };

};

#endif