#include <iostream>
#include "Game.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    std::string filename = argv[1];

    Game game;
    if (!game.getLevel(filename)) {
        cout << "Erreur lors de la lecture du fichier" << endl;
        return 1;
    }

    return 0;

};


// Game game;
//     vector<unique_ptr<Brick>> bricks;
//     int score;
//     int vie;

//     std::string filename = argv[1];
    
//     if (!game.getLevel(filename, bricks, score, vie)) {
//         cout << "Erreur lors de la lecture du fichier" << endl;
//         return 1;
//     }

//     return 0;