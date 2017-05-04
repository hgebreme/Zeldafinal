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
    // set all variables
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

Enemy::~Enemy(){} // no memory is dynamically allocated, so nothing must be done in the deconstructor

void Enemy::attack(){
        setFrame(getFrame() + 6); // switch the frame being rendered to the one of it attacking

        // check for collisions
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
    if(movecount % 5 == 0){ // change the direction that the enemy moves every 5 cycles through the main loop
        setDir(rand() % 4);
    }

    // set previous x and y positions
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

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
