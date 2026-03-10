#ifndef game_H
#define game_H
#include <string>


class game{
    public:
        game (int lives,int score);
        void getLevel(const std::string& filename); // déclaration de la fn de lecture de fichier
    private:
        int lives;
        int score;
};
#endif