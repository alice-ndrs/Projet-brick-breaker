#include <iostream>
#include "Ball.h"
#include "Constants.h"
#include "Message.h"
#include "Brick.h"
#include "Paddle.h"
#include <cmath>
using namespace std;

Ball::Ball (double x,double y,double r,double dx,double dy):
circle{{x,y}, r}, dx(dx), dy(dy)
{}

int Ball::check_Ball () const
{
    if ((circle.center.x - circle.r < 0 || arena_size < circle.center.x + circle.r) ||
    (circle.center.y < 0 || arena_size < circle.center.y + circle.r)){
        cout << message::ball_outside(circle.center.x, circle.center.y);
        return 1;
    }

    if(sqrt(dx*dx+dy*dy)>delta_norm_max){ 
        cout << message::invalid_delta(dx,dy);
        return 1;
    }

    return 0;
}

bool Ball::collision_ball (const Ball& other) const 
{
    return circle_intersects_circle(circle, other.circle);
}

bool Ball::collision_brick (const Brick& b) const 
{
    return circle_intersects_square(circle, b.getSquare());
}

bool Ball::collision_paddle (const Paddle& paddle) const 
{ 
    return circle_intersects_circle(circle, paddle.getCircle());
}