#include <iostream>
#include "Paddle.h"
#include "Message.h"
#include "Constants.h"
using namespace std;


Paddle::Paddle (double x,double y, double r):
x(x),y(y),r(r)
{}

int Paddle::check_Paddle(){
    if (y > 0) {
        cout << message::paddle_outside(x, y);
        return 1;
    }
    
    // Une partie du cercle doit être visible
    if (y + r <= 0) { 
        cout << message::paddle_outside(x, y);
        return 1;
    }

    // Les intersections avec l’axe x doivent exister
    double dx_squared = r * r - y * y; 
    if (dx_squared < 0) {
        cout << message::paddle_outside(x, y);
        return 1;
    }

    double dx = sqrt(dx_squared);
    double left  = x - dx;
    double right = x + dx;
    if (left < 0 || right > arena_size) {
        cout << message::paddle_outside(x, y);
        return 1;
    }

    return 0;
}