/* Contains the implementation for the Boss Class. It inherits from the Character class */

#ifndef BOSS_H
#define BOSS_H

class Boss: public Character {
    public:
        Boss(int, int, int, int, int, string, int, SDL_Rect*, int, int, bool, int, int, int, int, int, int); // constructor
        ~Boss(); // deconstructor
        void move(const Uint8*); // move the sprite
        void attack(); // attack

        // get and set private variables
        int getAttack1XPos();
        void setAttack1XPos(int);
        int getAttack1YPos();
        void setAttack1YPos(int);
        int getAttack2XPos();
        void setAttack2XPos(int);
        int getAttack2YPos();
        void setAttack2YPos(int);

    private:
        // Variables for the x and y position for its attack
        int attack1XPos;
        int attack1YPos;
        int attack2XPos;
        int attack2YPos;

};

#endif

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
