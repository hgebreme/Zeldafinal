/* Class implementation for link */

#ifndef LINK_H
#define LINK_H

class Link : public Character {
    public:
        Link(int, int, int, int, int, string, int, SDL_Rect*, int, int, bool, int, int, int, int, int, int);
        ~Link();
        void move(const Uint8*);
        void attack();
        int getInvinceTime();
        void setInvinceTime(int);

    private:
        int invinceTime; //number of frames that link is invincible
};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
