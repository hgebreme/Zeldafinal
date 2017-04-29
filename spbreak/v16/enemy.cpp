/* This file contains the implementation for the Enemy class */

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

Enemy::Enemy(int h=0, int x=0, int y=0, int px=0, int py=0, string path="empty", int n=0, SDL_Rect* sp=NULL, int f=0, int pf=0, bool a=true, int dir=0, int sx=0, int sy=0, int sw=0, int sh=0){
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
}

Enemy::~Enemy(){}

void Enemy::attack(){}

void Enemy::move(const Uint8* currentKeyStates){}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
