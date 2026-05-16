#include <iostream>
#include "Paddle.h"
#include "Message.h"
#include "Constants.h"
#include <cmath>
using namespace std;

//------------- Constructeur Paddle -------------

Paddle::Paddle (double x,double y, double r):
circle{{x,y}, r}
{}

//------------- Fonction de test pour la classe Paddle -------------

int Paddle::check_paddle(bool print_error, bool epsil) const
{
    // Les intersections avec l’axe x doivent exister
    double dx_squared = circle.r * circle.r - 
                        circle.center.y * circle.center.y; 
                        
    if (circle.center.y > 0 || dx_squared < 0) {
        if(print_error){
            cout << message::paddle_outside(circle.center.x, circle.center.y);
        }
        return 1;
    }
    
    // Une partie du cercle doit être visible
    if (circle.center.y + circle.r <= 0) { 
        if(print_error){
            cout << message::paddle_outside(circle.center.x, circle.center.y);
        }
        return 1;
    }

    double dx = sqrt(dx_squared);
    double left  = circle.center.x - dx;
    double right = circle.center.x + dx;
    double tol =epsil? 0:epsil_zero;

    if ((left < tol) || (right > arena_size - tol)) {
        if(print_error){
            cout << message::paddle_outside(circle.center.x, circle.center.y);
        }
        return 1;
    }

    return 0;
}