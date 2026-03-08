#ifndef brick_H
#define brick_H

class ball{
    public:
        ball (double x,double y,double r,double dx,double dy);
    private:
        double x,y; // position 
        double r; //rayon
        double dx,dy;// vecteur deplacement

};
#endif