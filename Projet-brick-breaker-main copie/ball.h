#ifndef BALL_H
#define BALL_H

class Ball{
    public:
        Ball (double x,double y,double r,double dx,double dy);
        double getX() const { return x; }
        double getY() const { return y; }
        double getR() const { return r; }
    private:
        double x,y; // position 
        double r; //rayon
        double dx,dy;// vecteur deplacement

};
#endif