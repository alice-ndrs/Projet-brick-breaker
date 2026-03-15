#ifndef PADDLE_H
#define PADDLE_H

class Paddle {
    public:
        Paddle (double x,double y, double r);
        double getX() const { return x; }
        double getY() const { return y; }
        double getR() const { return r; }
    private:
        double x,y; //position
        double r;// rayon 
};

#endif