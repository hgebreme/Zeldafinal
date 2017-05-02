/* Contains the implementation for the Link subclass */

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include"character.h"
#include"link.h"
#include"enemy.h"
#include"boss.h"
#include"ltexture.h"
#include"constants.h"
#include"constchars.h"

Link::Link(int h=0, int x=0, int y=0, int px=0, int py=0, string path=NULL, int n=0, SDL_Rect* sp=NULL, int f=0, int pf=0, bool a=true, int dir=0, int sx=0, int sy=0, int sw=0, int sh=0, int m=0){
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
    setInvinceTime(0);
}

Link::~Link(){}

void Link::move(const Uint8* currentKeyStates){
    if(currentKeyStates[SDL_SCANCODE_UP]){
        setYPos(getYPos() - SPEED); // move up
        if(getFrame() == 2){
            setFrame(3);
        }else{
            setFrame(2);
        }
    }else if(currentKeyStates[SDL_SCANCODE_DOWN]){
        setYPos(getYPos() + SPEED); // move down
        if(getFrame() == 0){
            setFrame(1);
        }else{
            setFrame(0);
        }
    }else if(currentKeyStates[SDL_SCANCODE_LEFT]){
        setXPos(getXPos() - SPEED); // move left
        if(getFrame() == 4){
            setFrame(5);
        }else{
            setFrame(4);
        }
    }else if(currentKeyStates[SDL_SCANCODE_RIGHT]){
        setXPos(getXPos() + SPEED); // move right;
        if(getFrame() == 6){
            setFrame(7);
        }else{
            setFrame(6);
        }
    }
}

void Link::attack(){
    SDL_Rect stretch = getStretch();
    SDL_Rect e0stretch = e0.getStretch();
    SDL_Rect e1stretch = e1.getStretch();
    SDL_Rect e2stretch = e2.getStretch();
    SDL_Rect bstretch = b.getStretch();

    if(getFrame() == 0 || getFrame() == 1){ // attack down
        setPrevFrame(getFrame());
        setFrame(8);
        if(stretch.y + stretch.h + 10 > e0stretch.y && stretch.y + stretch.h + 10 < e0stretch.y + e0stretch.h && stretch.x < e0stretch.x + e0stretch.w && stretch.x + stretch.w > e0stretch.x){
            e0.takeDamage();
        }
        if(stretch.y + stretch.h + 10 > e1stretch.y && stretch.y + stretch.h + 10 < e1stretch.y + e1stretch.h && stretch.x < e1stretch.x + e1stretch.w && stretch.x + stretch.w > e1stretch.x){
            e1.takeDamage();
        }
        if(stretch.y + stretch.h + 10 > e2stretch.y && stretch.y + stretch.h + 10 < e2stretch.y + e2stretch.h && stretch.x < e2stretch.x + e2stretch.w && stretch.x + stretch.w > e2stretch.x){
            e2.takeDamage();
        }
        if(stretch.y + stretch.h + 10 > bstretch.y && stretch.y + stretch.h + 10 < bstretch.y + bstretch.h && stretch.x < bstretch.x + bstretch.w && stretch.x + stretch.w > bstretch.x){
            b.takeDamage();
        }

    }else if(getFrame() == 2 || getFrame() == 3){ // attack up
        setPrevFrame(getFrame());
        setFrame(9);
        if(stretch.y - 10 < e0stretch.y + e0stretch.h && stretch.y - 10 > e0stretch.y && stretch.x < e0stretch.x + e0stretch.w && stretch.x + stretch.w > e0stretch.x){
            e0.takeDamage();
        }
        if(stretch.y - 10 < e1stretch.y + e1stretch.h && stretch.y - 10 > e1stretch.y && stretch.x < e1stretch.x + e1stretch.w && stretch.x + stretch.w > e1stretch.x){
            e1.takeDamage();
        }
        if(stretch.y - 10 < e2stretch.y + e2stretch.h && stretch.y - 10 > e2stretch.y && stretch.x < e2stretch.x + e2stretch.w && stretch.x + stretch.w > e2stretch.x){
            e2.takeDamage();
        }
        if(stretch.y - 10 < bstretch.y + bstretch.h && stretch.y - 10 > bstretch.y && stretch.x < bstretch.w + bstretch.w && stretch.x + stretch.w > bstretch.x){
            b.takeDamage();
        }
    
    }else if(getFrame() == 4 || getFrame() == 5){ // attack left
        setPrevFrame(getFrame());
        setFrame(10);
        if(stretch.x - 10 < e0stretch.x + e0stretch.w && stretch.x - 10 > e0stretch.x && stretch.y < e0stretch.y + e0stretch.h && stretch.y + stretch.h > e0stretch.y){
            e0.takeDamage();
        }
        if(stretch.x - 10 < e1stretch.x + e1stretch.w && stretch.x - 10 > e1stretch.x && stretch.y < e1stretch.y + e1stretch.h && stretch.y + stretch.h > e1stretch.y){
            e1.takeDamage();
        }
        if(stretch.x - 10 < e2stretch.x + e2stretch.w && stretch.x - 10 > e2stretch.x && stretch.y < e2stretch.y + e2stretch.h && stretch.y + stretch.h > e2stretch.y){
            e2.takeDamage();
        }
        if(stretch.x - 10 < bstretch.x + bstretch.w && stretch.x - 10 > bstretch.x && stretch.y < bstretch.y + bstretch.h && stretch.y + stretch.h > bstretch.y){
            b.takeDamage();
        }

    }else if(getFrame() == 6 || getFrame() == 7){ // attack right
        setPrevFrame(getFrame());
        setFrame(11);
        if(stretch.x + stretch.w + 10 > e0stretch.x && stretch.x + stretch.w + 10 < e0stretch.x + e0stretch.w && stretch.y < e0stretch.y + e0stretch.h && stretch.y + stretch.h > e0stretch.y){
            e0.takeDamage();
        }
        if(stretch.x + stretch.w + 10 > e1stretch.x && stretch.x + stretch.w + 10 < e1stretch.x + e1stretch.w && stretch.y < e1stretch.y + e1stretch.h && stretch.y + stretch.h > e1stretch.y){
            e1.takeDamage();
        }
        if(stretch.x + stretch.w + 10 > e2stretch.x && stretch.x + stretch.w + 10 < e2stretch.x + e2stretch.w && stretch.y < e2stretch.y + e2stretch.h && stretch.y + stretch.h > e2stretch.y){
            e2.takeDamage();
        }
        if(stretch.x + stretch.w + 10 > bstretch.x && stretch.x + stretch.w + 10 < bstretch.x + bstretch.w && stretch.y < bstretch.y + bstretch.h && stretch.y + stretch.h > bstretch.y){
            b.takeDamage();
        }
    }
}

int Link::getInvinceTime(){
    return invinceTime;
}

void Link::setInvinceTime(int t){
     invinceTime = t;
}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
