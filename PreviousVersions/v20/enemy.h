/* Contains the interface for the common enemy encountered */

#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public Character {
    public:
        Enemy(int, int, int, int, int, string, int, SDL_Rect*, int, int, bool, int, int, int, int, int, int);        
        ~Enemy();
        void move(const Uint8*);
        void attack();

    // don't need to add anything to private
};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
