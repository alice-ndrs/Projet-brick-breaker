#include <iostream>
using namespace std;

#ifndef brick_H
#define brick_H

class brick{
    public:
        brick(int t, double x, double y, double c, int h);
    private:
        int t;// type de brick parmis les 3
        double x,y;// position 
        double c;// taille des cotés
        int h; // niveau de resistance
};

#endif