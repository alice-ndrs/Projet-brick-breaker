#ifndef Game_H
#define Game_H
#include <string>


class Game{
    public:
        Game (int lives,int score);
        void getLevel(const std::string& filename); // déclaration de la fn de lecture de fichier
    private:
        int lives;
        int score;
};
#endif