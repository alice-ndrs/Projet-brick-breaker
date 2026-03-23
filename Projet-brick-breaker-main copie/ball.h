#ifndef BALL_H
#define BALL_H

#include "tools.h"

class Brick;
class Paddle;

class Ball{
    public:
        Ball (double x,double y,double r,double dx,double dy);
        int check_Ball ();
        bool collision_ball (const Ball& other) const;
        bool collision_brick (const Brick& b) const;
        bool collision_paddle (const Paddle& p) const;
        double getX() const { return circle.center.x; }
        double getY() const { return circle.center.y; }
        double getR() const { return circle.r; }
        const Circle& getCircle() const { return circle; }
        void clear() { circle = {{0,0}, 0}; dx = 0; dy = 0; }
    private:
        Circle circle;
        double dx,dy;// vecteur deplacement

};
#endif