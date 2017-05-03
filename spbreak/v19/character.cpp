/* This file contains the interface for the Character base class */

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include"character.h"
#include"ltexture.h"
using namespace std;

Character::Character(){
    setHealth(0);
    setXPos(0);
    setYPos(0);
    setPrevXPos(0);
    setPrevYPos(0);
    // initially dont do anything with LTexture
    setNumSprites(0);
    setSprites(NULL);
    setFrame(0);
    setPrevFrame(0);
    setAlive(false);
    setDir(0);
}

Character::~Character(){}

int Character::getHealth(){
    return health;
}

void Character::setHealth(int h){
    health = h;
}

int Character::getXPos(){
    return xpos;
}

void Character::setXPos(int x){
    xpos = x;
}

int Character::getYPos(){
    return ypos;
}

void Character::setYPos(int y){
    ypos = y;
}

int Character::getPrevXPos(){
    return prevxpos;
}

void Character::setPrevXPos(int x){
    prevxpos = x;
}

int Character::getPrevYPos(){
    return prevypos;
}

void Character::setPrevYPos(int y){
    prevypos = y;
}

int Character::getNumSprites(){
    return numSprites;
}

void Character::setNumSprites(int n){
    numSprites = n;
}

int Character::getFrame(){
    return frame;
}

void Character::setFrame(int f){
    frame = f;
}

int Character::getPrevFrame(){
    return prevframe;
}

void Character::setPrevFrame(int f){
    prevframe = f;
}

bool Character::getAlive(){
    return alive;
}

void Character::setAlive(bool a){
    alive = a;
}

int Character::getDir(){
    return dir;
}

void Character::setDir(int d){
    dir = d;
}

LTexture Character::getLTexture(){
    return text;
}

bool Character::setLTexture(string path){
    if(path.compare("empty") != 0)
        return text.loadFromFile(path);
}

SDL_Rect* Character::getSprites(){
    return sprites;
}

void Character::setSprites(SDL_Rect* s){
    sprites = s;
}

SDL_Rect Character::getStretch(){
    return stretch;
}

void Character::setStretch(int x, int y, int w, int h){
    stretch.x = x;
    stretch.y = y;
    stretch.w = w;
    stretch.h = h;
}

void Character::setStretchX(int x){
    stretch.x = x;
}

void Character::setStretchY(int y){
    stretch.y = y;
}

int Character::getStretchW(){
    return stretch.w;
}

int Character::getStretchH(){
    return stretch.h;
}

void Character::renderChar(SDL_Renderer* r){
    SDL_Rect* rect = getSprites();
    SDL_Rect spriteRender = rect[frame];
    SDL_Rect stret = getStretch();
    SDL_RenderCopy(r, text.texture(), &spriteRender, &stret);
}

void Character::takeDamage(){
    if(getHealth() > 0){
        setHealth(getHealth() - 1);
    }
    if(getHealth() <= 0){
        setAlive(false);
    } 

}

int Character::getMaxHealth(){
    return maxHealth;
}

void Character::setMaxHealth(int m){
    maxHealth = m;
}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
