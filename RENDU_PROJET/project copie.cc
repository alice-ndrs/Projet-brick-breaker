#include "Game.h"

int main(int argc, char* argv[]) 
{
    Game game;

    if (argc < 2) return 1;

    std::string filename = argv[1];

    if (!game.getLevel(filename)) {
        return 1;
    }

    return 0;
};