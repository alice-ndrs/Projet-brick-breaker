#include <iostream>
#include "Brick.h"
#include "Message.h"
#include "Constants.h"
#include "Paddle.h"
#include <cmath>
using namespace std;


Brick :: Brick (int t, double x, double y, double c, int h ):
t(t),x(x),y(y),c(c),h(h)
{}

int Brick::check_Brick(){
    if (t>2 or t<0){
        cout << message::invalid_brick_type(t);
        return 1;
    }
    
    if ((x - c/2 < 0 || arena_size < x + c/2) ||
    (y - c/2 < 0 || arena_size < y + c/2)){
        cout << message::brick_outside(x,y);
        return 1;
    }

    if (c<brick_size_min){
        cout << message::invalid_brick_size(c);
        return 1;
    }

    if (t==0) {
        if (h<1 || h>7){
            cout << message::invalid_hit_points(h);
            return 1;
        }
    }

    return 0;
}

bool Brick::collision_brick (const Brick& other) const {
    double dx = abs(x - other.x);
    double dy = abs(y - other.y);
    double limit = (c + other.c) / 2.0;

    return (dx < limit && dy < limit);
}

// VOIR SI ON PEUT OPTMISER !!
bool Brick::collision_paddle (const Paddle& p) const { 
    double half = c / 2.0;

    // bornes du carré
    double left   = x - half;
    double right  = x + half;
    double bottom = y - half;
    double top    = y + half;

    // centre du cercle
    double cx = p.getX();
    double cy = p.getY();
    double r  = p.getR();

    // point du carré le plus proche du centre du cercle
    double closestX = std::max(left, std::min(cx, right));
    double closestY = std::max(bottom, std::min(cy, top));

    // distance au carré
    double dx = cx - closestX;
    double dy = cy - closestY;

    return (dx*dx + dy*dy) < (r*r);
}