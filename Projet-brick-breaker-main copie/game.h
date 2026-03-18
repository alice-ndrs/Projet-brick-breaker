#ifndef Game_H
#define Game_H
#include <iostream>
#include <vector>
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"
#include <string>


class Game{
    public:
        Game (int lives = 0,int score = 0);
        void getLevel (const std::string& filename); // déclaration de la fn de lecture de fichier
        int check_Collisions () const;
        bool collision_bricks() const;
        bool collision_brick_paddle() const;
    private:
        int lives;
        int score;
        vector <Brick> bricks;
        vector <Ball> balls;
        Paddle paddle;
};
#endif