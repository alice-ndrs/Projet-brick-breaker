#include <iostream>
#include "Game.h"
using namespace std;

int main(){
    Game game;

    if (!game.getLevel("t00.txt")) {
        cout << "Erreur lors de la lecture du fichier" << endl;
        return 1;
    }

    return 0;

};