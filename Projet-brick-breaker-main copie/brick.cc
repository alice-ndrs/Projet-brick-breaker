#include <iostream>
#include "Brick.h"
#include "Message.h"
#include "Constants.h"
#include "Paddle.h"
using namespace std;


Brick::Brick(BrickType t, double x, double y, double c) :
    t(t), square{{x,y}, c}
{}

int Brick::check_Brick() const // mettre en booléen
{ 
    int type_value = static_cast<int>(t);
    if (type_value < 0 || type_value > 2) {
        cout << message::invalid_brick_type(type_value);
        return 1;
    }
    
    if ((square.center.x - square.side/2 < 0 || arena_size < square.center.x + square.side/2) ||
    (square.center.y - square.side/2 < 0 || arena_size < square.center.y + square.side/2)) {
        cout << message::brick_outside(square.center.x, square.center.y);
        return 1;
    }

    if (square.side<brick_size_min) {
        cout << message::invalid_brick_size(square.side);
        return 1;
    }

    return check_specific();
}

int Rainbow_brick::check_specific() const 
{
    if (hit_points<1 || hit_points>7) {
        cout << message::invalid_hit_points(hit_points);
        return 1;
    }
    return 0;
}

bool Brick::collision_brick (const Brick& other) const 
{
    return square_intersects_square(square, other.square);
}

bool Brick::collision_paddle (const Paddle& p) const 
{
    return square_intersects_circle(square, p.getCircle());
}