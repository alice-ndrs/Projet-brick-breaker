#ifndef Game_H
#define Game_H
#include <iostream>
#include <vector>
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"
#include <string>

enum EtatLecture {
    SCORE,
    LIVES,
    PADDLE,
    NB_BRICKS,
    BRICKS,
    NB_BALLS,
    BALLS,
    FIN
};

class Game{
    public:
        Game (int lives = 0,int score = 0);
        bool getLevel (const std::string& filename); // déclaration de la fn de lecture de fichier
        int check_Collisions () const;
        bool collision_bricks() const;
        bool collision_balls() const;
        bool collision_ball_brick() const;
        bool collision_ball_paddle() const;
        bool collision_brick_paddle() const;
        void reset();
        bool decodage_ligne(std::istringstream& data);

        //méthodes à écrire dans game.cc
        bool decodage_score(std::istringstream& data);
        bool decodage_lives(std::istringstream& data);
        bool decodage_paddle(std::istringstream& data);
        bool decodage_nb_bricks(std::istringstream& data);
        bool decodage_brick(std::istringstream& data);
        bool decodage_nb_balls(std::istringstream& data);
        bool decodage_ball(std::istringstream& data);

    private:
        int lives;
        int score;
        vector <Brick> bricks;
        vector <Ball> balls;
        Paddle paddle;
        EtatLecture etat;
        unsigned int total;
        unsigned int count;
};
#endif