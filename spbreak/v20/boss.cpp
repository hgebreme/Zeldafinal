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
    setAttack1XPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8);
    setAttack1YPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11);
    setAttack2XPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8);
    setAttack2YPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11); 
 
}

Boss::~Boss(){}

void Boss::attack(){
    int w = 100;
    int h = 100;
    SDL_Rect damageBlock1 = {getAttack1XPos(), getAttack1YPos(), w, h};
    SDL_Rect damageBlock2 = {getAttack2XPos(), getAttack2YPos(), w, h};
    if(movecount % 50 > 37){
        SDL_SetRenderDrawColor(gRenderer, 235, 14, 14, 255);
    }else{
        SDL_SetRenderDrawColor(gRenderer, 9, 39, 233, 255);
    }
    SDL_RenderFillRect(gRenderer, &damageBlock1);
    SDL_RenderFillRect(gRenderer, &damageBlock2);
    SDL_RenderPresent(gRenderer);
    if(movecount % 50 > 37 && link.getXPos() + link.getStretchW() > getAttack1XPos() && link.getXPos() < getAttack1XPos() + w && link.getYPos() + link.getStretchH() > getAttack1YPos() && link.getYPos() < getAttack1YPos() + h){
        if(link.getInvinceTime() == 0){
            link.takeDamage();
            link.setInvinceTime(25);
        }
    }
    if(movecount % 50 > 37 && link.getXPos() + link.getStretchW() > getAttack2XPos() && link.getXPos() < getAttack2XPos() + w && link.getYPos() + link.getStretchH() > getAttack2YPos() && link.getYPos() < getAttack2YPos() + h){
        if(link.getInvinceTime() == 0){
            link.takeDamage();
            link.setInvinceTime(25);
        }
    }
}

void Boss::move(const Uint8* currentKeyStates){
    if(movecount % 5 == 0){
        setDir(rand() % 4);
    }

    setPrevXPos(getXPos());
    setPrevYPos(getYPos());

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
