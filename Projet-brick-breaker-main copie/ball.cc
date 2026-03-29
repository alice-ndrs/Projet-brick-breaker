#include <iostream>
#include "Ball.h"
#include "Brick.h"
#include "Message.h"
using namespace std;

//------------- Constructeur Ball -------------

Ball::Ball (double x,double y,double r,double dx,double dy):
circle{{x,y}, r}, dx(dx), dy(dy), active(true)
{}

//------------- Fonction de test pour la classe Ball -------------

int Ball::check_Ball () const   //verifie si Ball est dans l'arene et 
{                                 // et sa vitesse est conforme
    bool outside_x = (circle.center.x - circle.r < 0) 
                    || (circle.center.x + circle.r > arena_size);

    bool outside_y = (circle.center.y < 0) 
                    || (circle.center.y + circle.r > arena_size);
              
    if (outside_x || outside_y){
        cout << message::ball_outside(circle.center.x, circle.center.y);
        return 1;
    }

    if(sqrt(dx*dx + dy*dy)>delta_norm_max){ // verification norme delta
        cout << message::invalid_delta(dx,dy);
        return 1;
    }

    return 0;
}

// --- Détection de collisions ---

bool Ball::collision_ball (const Ball& other) const // 2 Ball se superpose
{
    return circle_intersects_circle(circle, other.circle);
}


bool Ball::collision_brick (const Brick& b) const //superposition Brick et Ball
{
    return circle_intersects_square(circle, b.getSquare());
}


bool Ball::collision_paddle (const Paddle& paddle) const //superposition Paddle
{                                                        //    et Ball
    return circle_intersects_circle(circle, paddle.getCircle());
}


void Ball::reset(const Paddle& p) { //replace la Ball sur le Paddle
    double X = p.getX();
    double Y = p.getY() + p.getR() + new_ball_radius + epsil_zero;
    
    circle.center = {X, Y};
    circle.r = new_ball_radius;
    dx = 0;
    dy = new_ball_delta_norm;
    active = true;
}

