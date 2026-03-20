#ifndef BALL_H
#define BALL_H

class Brick;
class Paddle;

class Ball{
    public:
        Ball (double x,double y,double r,double dx,double dy);
        int check_Ball ();
        bool collision_ball (const Ball& other) const;
        bool collision_brick (const Brick& b) const;
        bool collision_paddle (const Paddle& p) const;
        double getX() const { return x; }
        double getY() const { return y; }
        double getR() const { return r; }
        void clear() { x = 0; y = 0; r = 0; dx = 0; dy = 0; }
    private:
        double x,y; // position 
        double r; //rayon
        double dx,dy;// vecteur deplacement

};
#endif