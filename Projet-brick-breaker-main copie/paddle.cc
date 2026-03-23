#include <iostream>
#include "Paddle.h"
#include "Message.h"
#include "Constants.h"
using namespace std;


Paddle::Paddle (double x,double y, double r):
circle{{x,y}, r}
{}

int Paddle::check_Paddle(){
    if (circle.center.y > 0) {
        cout << message::paddle_outside(circle.center.x, circle.center.y);
        return 1;
    }
    
    // Une partie du cercle doit être visible
    if (circle.center.y + circle.r <= 0) { 
        cout << message::paddle_outside(circle.center.x, circle.center.y);
        return 1;
    }

    // Les intersections avec l’axe x doivent exister
    double dx_squared = circle.r * circle.r - circle.center.y * circle.center.y; 
    if (dx_squared < 0) {
        cout << message::paddle_outside(circle.center.x, circle.center.y);
        return 1;
    }

    double dx = sqrt(dx_squared);
    double left  = circle.center.x - dx;
    double right = circle.center.x + dx;
    if (left < 0 || right > arena_size) {
        cout << message::paddle_outside(circle.center.x, circle.center.y);
        return 1;
    }

    return 0;
}