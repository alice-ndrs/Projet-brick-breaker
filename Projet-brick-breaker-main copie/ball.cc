#include <iostream>
#include "Ball.h"
#include "Constants.h"
#include "Message.h"
using namespace std;

Ball::Ball (double x,double y,double r,double dx,double dy):
x(x),y(y),r(r),dx(dx),dy(dy)
{}

int Ball::check_Ball (Ball b){
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