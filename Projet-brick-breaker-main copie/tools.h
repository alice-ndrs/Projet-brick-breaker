#ifndef TOOLS_H
#define TOOLS_H

#include "Message.h"
#include "Constants.h"
using namespace std;

class Ball;
class Brick;
class Paddle;

struct Point {
    double x;
    double y;
};

struct Circle {
    Point center;
    double r;
};

struct Square {
    Point center;
    double side;
};

bool intersects(const Square& s1, const Square& s2);
bool brick_intersects_brick(const Brick& b1, const Brick& b2);
bool ball_intersects_ball(const Ball& ball1, const Ball& ball2);
bool ball_intersects_brick(const Ball& ball, const Brick& brick);
bool ball_intersects_paddle(const Ball& ball, const Paddle& paddle);
bool brick_intersects_paddle(const Brick& brick, const Paddle& paddle);

#endif