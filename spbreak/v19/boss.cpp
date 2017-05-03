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
    setAttackXPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8);
    b.setAttackYPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11); 
}

Boss::~Boss(){}

void Boss::attack(){
    int w = 100;
    int h = 100;
    SDL_Rect damageBlock = {getAttackXPos(), getAttackYPos(), w, h};
    if(movecount % 50 > 37){
        SDL_SetRenderDrawColor(gRenderer, 235, 14, 14, 255);
    }else{
        SDL_SetRenderDrawColor(gRenderer, 9, 39, 233, 255);
    }
    SDL_RenderFillRect(gRenderer, &damageBlock);
    SDL_RenderPresent(gRenderer);
    if(movecount % 50 > 37 && link.getXPos() + link.getStretchW() > getAttackXPos() && link.getXPos() < getAttackXPos() + w && link.getYPos() + link.getStretchH() > getAttackYPos() && link.getYPos() < getAttackYPos() + h){
        if(link.getInvinceTime() == 0){
            link.takeDamage();
            link.setInvinceTime(25);
        }
    }
}

void Boss::move(const Uint8* currentKeyStates){}

int Boss::getAttackXPos(){
    return attackXPos;
}

void Boss::setAttackXPos(int x){
    attackXPos = x;
}

int Boss::getAttackYPos(){
    return attackYPos;
}

void Boss::setAttackYPos(int y){
    attackYPos = y;
}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
