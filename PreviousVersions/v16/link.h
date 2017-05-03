/* Class implementation for link */

#ifndef LINK_H
#define LINK_H

class Link : public Character {
    public:
        Link(int, int, int, int, int, string, int, SDL_Rect*, int, int, bool, int, int, int, int, int);//constructor
        ~Link();// deconstructor 
        void move(const Uint8*); // input needed to move character
        void attack();// function called when user presses A

    // don't need to add anything to private
};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
