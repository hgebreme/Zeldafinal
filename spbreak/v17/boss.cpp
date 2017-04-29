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
    setHealth(h);
    setXPos(x);
    setYPos(y);
    setPrevXPos(px);
    setPrevYPos(py);
    setLTexture(path);
    setNumSprites(n);
    setSprites(sp);
    setFrame(f);
    setPrevFrame(pf);
    setAlive(a);
    setDir(dir);
    setStretch(sx, sy, sw, sh);
    setMaxHealth(m);
}

Boss::~Boss(){}

void Boss::attack(){
    int x = (rand() % 3*fullScreen.w/5) + fullScreen.w / 8;
    int y = (rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h / 11;
    int w = 80;
    int h = 80;
    SDL_Rect damageBlock = {x, y, w, h};
    SDL_SetRenderDrawColor(gRenderer, 235, 14, 14, 255);
    SDL_RenderFillRect(gRenderer, &damageBlock);
    SDL_RenderPresent(gRenderer);
    if(link.getXPos() > x && link.getXPos() < x + w && link.getYPos() > y && link.getYPos() < y + h){
        link.takeDamage();
    }
}

void Boss::move(const Uint8* currentKeyStates){}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
