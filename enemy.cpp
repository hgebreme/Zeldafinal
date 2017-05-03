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

Enemy::Enemy(int h=0, int x=0, int y=0, int px=0, int py=0, string path="empty", int n=0, SDL_Rect* sp=NULL, int f=0, int pf=0, bool a=true, int dir=0, int sx=0, int sy=0, int sw=0, int sh=0, int m=0){
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

Enemy::~Enemy(){}

void Enemy::attack(){
        setFrame(getFrame() + 6);
        if(link.getXPos() < getXPos() + getStretch().w && link.getXPos() > getXPos() && link.getYPos() > getYPos() && link.getYPos() < getYPos() + getStretch().h){
            if(link.getInvinceTime() == 0){
                link.takeDamage();
                link.takeDamage(); // doubles the damage. makes it harder (this line can be removed to make the game easier)
                link.setInvinceTime(10);
            }
        }else if(link.getXPos() < getXPos() + getStretch().w && link.getXPos() > getXPos() && link.getYPos() + link.getStretch().h > getYPos() && link.getYPos() + link.getStretch().h < getYPos() + getStretch().h){
            if(link.getInvinceTime() == 0){
                link.takeDamage(); // doubles the damage. makes it harder (this line can be removed to make the game easier)
                link.takeDamage();
                link.setInvinceTime(10);
            }
        }else if(link.getXPos() + link.getStretch().w > getXPos() && link.getXPos() + link.getStretch().w < getXPos() + getStretch().w && link.getYPos() + link.getStretch().h > getYPos() && link.getYPos() + link.getStretch().h < getYPos() + getStretch().h){
            if(link.getInvinceTime() == 0){
                link.takeDamage();
                link.takeDamage(); // doubles the damage. makes it harder (this line can be removed to make the game easier)
                link.setInvinceTime(10);
            }
        }else if(link.getXPos() + link.getStretch().w > getXPos() && link.getXPos() + link.getStretch().w < getXPos() + getStretch().w && link.getYPos() > getYPos() && link.getYPos() < getYPos() + getStretch().h){
            if(link.getInvinceTime() == 0){
                link.takeDamage();
                link.takeDamage(); // doubles the damage. makes it harder (this line can be removed to make the game easier)
                link.setInvinceTime(10);
            }
        }
}

void Enemy::move(const Uint8* currentKeyStates){
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

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
