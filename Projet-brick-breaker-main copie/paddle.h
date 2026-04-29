#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

class Paddle 
{
public:
    Paddle (double x = 0,double y = 0, double r = 0); // constructeur par défaut

    int check_paddle(bool print_error=true) const;

    // --- Accesseurs ---
    double getX() const { return circle.center.x; }
    double getY() const { return circle.center.y; }
    double getR() const { return circle.r; }
    void setX(double x) { circle.center.x = x; }
    const Circle& getCircle() const { return circle; }
    
private:
    Circle circle;
};

#endif