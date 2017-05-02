/* Contains the implementation for the Boss Class. It inherits from the Character class */

#ifndef BOSS_H
#define BOSS_H

class Boss: public Character {
    public:
        Boss(int, int, int, int, int, string, int, SDL_Rect*, int, int, bool, int, int, int, int, int, int);
        ~Boss();
        void move(const Uint8*);
        void attack();
        int getAttackXPos();
        void setAttackXPos(int);
        int getAttackYPos();
        void setAttackYPos(int);

    private:
        // Variables for the x and y position for its attack
        int attackXPos;
        int attackYPos;

};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
