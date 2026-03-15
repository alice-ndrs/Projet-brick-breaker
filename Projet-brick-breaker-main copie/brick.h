#include <iostream>
using namespace std;

#ifndef BRICK_H
#define BRICK_H

class Brick{
    public:
        Brick(int t, double x, double y, double c, int h=0);
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