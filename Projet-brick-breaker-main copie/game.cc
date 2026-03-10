#include <iostream>
#include "game.h"
#include <fstream>
#include <sstream>
using namespace std;

void game::getLevel(const string& filename){ // méthode de lecture de fichier
    ifstream file(filename);

    if (file.fail()) {
        // afficher mess erreur "Erreur ouverture fichier"
    }

    string line;
    while (getline(file>>ws, line)) {
        istringstream data(line);
        // lire les données, créer les objets nécessaires
    }
}

// getLevel est responsable de lire le fichier et vérifier que son contenu est correct.
// Les classes Brick, Ball, Paddle, etc. sont responsables de vérifier que leurs paramètres sont valides.


game::game (int lives,int score):
lives(lives),score(score)
{}