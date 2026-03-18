#include <iostream>
#include "Paddle.h"
using namespace std;

#ifndef BRICK_H
#define BRICK_H

class Brick{
    public:
        Brick(int t, double x, double y, double c, int h=0);
        int check_Brick(Brick b);
        bool collision_brick (const Brick& other) const; //check what does const mean
        bool collision_paddle (const Paddle& p) const;
        double getX() const { return x; }
        double getY() const { return y; }
        double getC() const { return c; }
    private:
        int t;// type de brick parmis les 3
        double x,y;// position 
        double c;// taille des cotés
        int h; // niveau de resistance
};

#endif