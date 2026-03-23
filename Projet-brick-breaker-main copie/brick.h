#include <iostream>
#include "Paddle.h"
#include "tools.h"
using namespace std;

#ifndef BRICK_H
#define BRICK_H

class Brick{
    public:
        Brick(double t, double x, double y, double c, double h=0);
        int check_Brick();
        bool collision_brick (const Brick& other) const; //check what does const mean
        bool collision_paddle (const Paddle& p) const;
        double getX() const { return square.center.x; }
        double getY() const { return square.center.y; }
        double getC() const { return square.side; }
        const Square& getSquare() const { return square; }
        void clear() { t = 0; square = {{0,0}, 0}; h = 0; }
    protected:
        double t;// type de brick parmis les 3
        Square square;// position et taille
        double h; // niveau de resistance
};

class Rainbow_brick : public Brick {
    public :
        Rainbow_brick(int t, double x, double y, double c, int h) : Brick(t, x, y, c, h) {}
    private : 
        int hit_points;
};

class Ball_brick : public Brick {
    public :
        Ball_brick(int t, double x, double y, double c) : Brick(t, x, y, c) {}
    private :
        double new_ball_radius;
};

class Split_brick : public Brick {
    public :
        Split_brick(int t, double x, double y, double c) : Brick(t, x, y, c) {}
    private :
        double split_brick_gap;
};

#endif