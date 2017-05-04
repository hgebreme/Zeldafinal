/* This file contains the implementation for the Boss class */

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include"ltexture.h"
#include"character.h"
#include"enemy.h"
#include"link.h"
#include"boss.h"
#include"constants.h"
#include"constchars.h"

Boss::Boss(int h=0, int x=0, int y=0, int px=0, int py=0, string path="empty", int n=0, SDL_Rect* sp=NULL, int f=0, int pf=0, bool a=true, int dir=0, int sx=0, int sy=0, int sw=0, int sh=0, int m=0){
    // initialize variables
    setHealth(h); // heath
    setXPos(x); // x position
    setYPos(y); // y position
    setPrevXPos(px); // previous x position
    setPrevYPos(py); // previous y position
    setLTexture(path); // LTexture (texture made from image specified by path)
    setNumSprites(n); // number of sprites
    setSprites(sp); // SDL_Rects that contains the clips for the sprites
    setFrame(f); // which sprite to render
    setPrevFrame(pf); // previous sprite
    setAlive(a); // alive or dead
    setDir(dir); // direction to move
    setStretch(sx, sy, sw, sh); // SDL_Rect that stretches the sprite to correct size
    setMaxHealth(m); // maximum health
    setAttack1XPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8); // x position of attack rectangle 1
    setAttack1YPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11); // y position of attack rectangle 1
    setAttack2XPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8); // x position of attack rectangle 2
    setAttack2YPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11);  // y position of attack rectangle 2
 
}

Boss::~Boss(){} // nothing is dynamically allocated within Boss class, so nothing must be deconstructed

void Boss::attack(){
    int w = 100; // width of attack rectanges
    int h = 100; // height of attack rectangles

    // set SDL_Rects for attacks
    SDL_Rect damageBlock1 = {getAttack1XPos(), getAttack1YPos(), w, h};
    SDL_Rect damageBlock2 = {getAttack2XPos(), getAttack2YPos(), w, h};

    // determine if rectangle is blue (safe) or red (dangerous)
    if(movecount % 50 > 37){
        SDL_SetRenderDrawColor(gRenderer, 235, 14, 14, 255); // red (dangerous)
    }else{
        SDL_SetRenderDrawColor(gRenderer, 9, 39, 233, 255); // blue (safe)
    }

    // render rectangles
    SDL_RenderFillRect(gRenderer, &damageBlock1);
    SDL_RenderFillRect(gRenderer, &damageBlock2);
    SDL_RenderPresent(gRenderer);

    // Damage link
    if(movecount % 50 > 37 && link.getXPos() + link.getStretchW() > getAttack1XPos() && link.getXPos() < getAttack1XPos() + w && link.getYPos() + link.getStretchH() > getAttack1YPos() && link.getYPos() < getAttack1YPos() + h){
        if(link.getInvinceTime() == 0){
            link.takeDamage();
            link.takeDamage(); // doubles the damage. makes it harder (this line can be removed to make the game easier)
            link.setInvinceTime(25);
        }
    }
    if(movecount % 50 > 37 && link.getXPos() + link.getStretchW() > getAttack2XPos() && link.getXPos() < getAttack2XPos() + w && link.getYPos() + link.getStretchH() > getAttack2YPos() && link.getYPos() < getAttack2YPos() + h){
        if(link.getInvinceTime() == 0){
            link.takeDamage();
            link.takeDamage(); // doubles the damage. makes it harder (this line can be removed to make the game easier)
            link.setInvinceTime(25);
        }
    }
}

void Boss::move(const Uint8* currentKeyStates){
    // randomize direction every 5 cycles through main loop
    if(movecount % 5 == 0){
        setDir(rand() % 4);
    }

    // set previous x and y position
    setPrevXPos(getXPos());
    setPrevYPos(getYPos());

    // set new x and y positions
    switch(getDir()){
        case 0:
            setXPos(getXPos() + SPEED);
            setFrame(3);
            break;
        case 1:
            setXPos(getXPos() - SPEED);
            setFrame(1);
            break;
        case 2:
            setYPos(getYPos() + SPEED);
            setFrame(0);
            break;
        case 3:
            setYPos(getYPos() - SPEED);
            setFrame(2);
            break;
    }

}

int Boss::getAttack1XPos(){
    return attack1XPos;
}

void Boss::setAttack1XPos(int x){
    attack1XPos = x;
}

int Boss::getAttack1YPos(){
    return attack1YPos;
}

void Boss::setAttack1YPos(int y){
    attack1YPos = y;
}

int Boss::getAttack2XPos(){
    return attack2XPos;
}

void Boss::setAttack2XPos(int x){
    attack2XPos = x;
}

int Boss::getAttack2YPos(){
    return attack2YPos;
}

void Boss::setAttack2YPos(int y){
    attack2YPos = y;
}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
