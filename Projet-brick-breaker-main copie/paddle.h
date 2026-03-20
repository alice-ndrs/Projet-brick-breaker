#ifndef PADDLE_H
#define PADDLE_H

class Paddle {
    public:
        Paddle (double x = 0,double y = 0, double r = 0); // constructeur par défaut
        int check_Paddle();
        double getX() const { return x; }
        double getY() const { return y; }
        double getR() const { return r; }
    private:
        double x,y; //position
        double r;// rayon 
};

#endif