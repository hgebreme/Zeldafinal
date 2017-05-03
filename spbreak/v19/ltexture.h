/* This is the implementation for the LTexture class. It is a texture wrapper class
 * This is taken from lazyfoo.net
 */

#ifndef LTEXTURE_H
#define LTEXTURE_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
using namespace std;

class LTexture {
    public:
        LTexture(); // initializes valiables
        ~LTexture(); // deallocates memory
        bool loadFromFile(string); // loads image at specified path
        void free(); // deallocates texture
        void render(int x, int y, SDL_Rect* clip = NULL); // renders texture at given point
        int getWidth(); // returns width
        int getHeight(); // returns height
        SDL_Texture* texture(); // returns texture

    private:
        SDL_Texture* mTexture; // actual hardware texture
        int mWidth; // image width
        int mHeight; // image height
};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
