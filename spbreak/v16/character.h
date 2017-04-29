/* Contains the interface for the base class Character */

#ifndef CHARACTER_H
#define CHARACTER_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include"ltexture.h"
using namespace std;

class Character{
    public:
        Character(); // constructor
        ~Character(); // deconstructor

        // get and set functions for each of the private variables
        int getHealth();
        void setHealth(int);
        int getXPos();
        void setXPos(int);
        int getYPos();
        void setYPos(int);
        int getPrevXPos();
        void setPrevXPos(int);
        int getPrevYPos();
        void setPrevYPos(int);
        int getNumSprites();
        void setNumSprites(int);
        int getFrame();
        void setFrame(int);
        int getPrevFrame();
        void setPrevFrame(int);
        bool getAlive();
        void setAlive(bool);
        int getDir();
        void setDir(int);
        LTexture getLTexture();
        bool setLTexture(string); // sets LTexture from a path to an image
        SDL_Rect* getSprites();
        void setSprites(SDL_Rect*);
        SDL_Rect getStretch();
        void setStretch(int, int, int, int);
        void setStretchX(int);
        void setStretchY(int);

        // other funtions
        void renderChar(SDL_Renderer*); // render the character
        void takeDamage();

        // virtual funtions
        virtual void move(const Uint8*) = 0; // virtual funtion, move character
        virtual void attack() = 0; // virtual funtion, attack

    private:
        int health; // character's health (in half hearts)
        int xpos; // x coordinate on screen
        int ypos; // y coordinate on screen
        int prevxpos; // previous x coordinate
        int prevypos; // previous y coordinate
        LTexture text; // texture from which sprites come
        int numSprites; // number of sprites used for character
        SDL_Rect* sprites; // array of SDL_Rects, each one represents a sprite
        int frame; // determines which sprite to load;
        int prevframe; // previous frame
        bool alive; // keeps track of whether the character is alive
        int dir; // determines which direction to move the character
        SDL_Rect stretch; // stretches the sprite to the appropriate size
};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
