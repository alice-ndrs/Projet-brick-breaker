#include <iostream>
#include "Ball.h"
#include "Constants.h"
#include "Message.h"
#include "Brick.h"
#include "Paddle.h"
#include <cmath>
using namespace std;

Ball::Ball (double x,double y,double r,double dx,double dy):
x(x),y(y),r(r),dx(dx),dy(dy)
{}

int Ball::check_Ball (){
    if ((x - r < 0 || arena_size < x + r) || // normalement choses fausses réglées
    (y < 0 || arena_size < y + r)){
        cout << message::ball_outside(x,y);
        return 1;
    }

    if(sqrt(dx*dx+dy*dy)>delta_norm_max){ // REGLER CE MERDIER
        cout << message::invalid_delta(dx,dy);
        return 1;
    }

    return 0;
}

bool Ball::collision_ball (const Ball& other) const {
    double dx = x - other.x;
    double dy = y - other.y;

    double distance_sq = dx*dx + dy*dy;
    double radius_sum = r + other.r;

    return distance_sq < radius_sum * radius_sum;
}

bool Ball::collision_brick (const Brick& b) const {
    double half = b.getC() / 2.0;

    double left   = b.getX() - half;
    double right  = b.getX() + half;
    double bottom = b.getY() - half;
    double top    = b.getY() + half;
    
    double closestX = max(left, min(x, right));
    double closestY = max(bottom, min(y, top));
    
    double dx = x - closestX;
    double dy = y - closestY;

    return (dx*dx + dy*dy) < (r * r);
}

bool Ball::collision_paddle (const Paddle& paddle) const { 
    double dx = x - paddle.getX();
    double dy = y - paddle.getY();

    double distance_sq = dx * dx + dy * dy;
    double radius_sum = r + paddle.getR();

    return distance_sq < radius_sum * radius_sum;
}