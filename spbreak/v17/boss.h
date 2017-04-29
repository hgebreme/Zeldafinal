/* Contains the implementation for the Boss Class. It inherits from the Character class */

#ifndef BOSS_H
#define BOSS_H

class Boss: public Character {
    public:
        Boss(int, int, int, int, int, string, int, SDL_Rect*, int, int, bool, int, int, int, int, int, int);
        ~Boss();
        void move(const Uint8*);
        void attack();

    // don't need to add anything to private

};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
