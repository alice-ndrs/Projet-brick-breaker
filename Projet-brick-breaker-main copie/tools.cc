#include "tools.h"
#include "Message.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "Constants.h"
#include <cmath>
using namespace std;

bool intersects(const Square& s1, const Square& s2) {
    double dx = abs(s1.center.x - s2.center.x);
    double dy = abs(s1.center.y - s2.center.y);
    double limit = (s1.side + s2.side) / 2.0;
    return (dx < limit && dy < limit);
}
// TRUC CHELOU ICI À REVOIR


bool brick_intersects_brick(const Brick& b1, const Brick& b2) {
    return intersects(b1.getSquare(), b2.getSquare());
}


bool ball_intersects_ball(const Ball& ball1, const Ball& ball2) {
    const Circle& c1 = ball1.getCircle();
    const Circle& c2 = ball2.getCircle();
    double dx = c1.center.x - c2.center.x;
    double dy = c1.center.y - c2.center.y;
    double distance_sq = dx*dx + dy*dy;
    double radius_sum = c1.r + c2.r;
    return distance_sq < radius_sum * radius_sum;
}

bool ball_intersects_brick(const Ball& ball, const Brick& brick) {
    const Square& s = brick.getSquare();
    const Circle& c = ball.getCircle();
    double half = s.side / 2.0;
    double left = s.center.x - half;
    double right = s.center.x + half;
    double bottom = s.center.y - half;
    double top = s.center.y + half;
    
    double closestX = max(left, min(c.center.x, right));
    double closestY = max(bottom, min(c.center.y, top));
    
    double dx = c.center.x - closestX;
    double dy = c.center.y - closestY;
    
    return (dx*dx + dy*dy) < (c.r * c.r);
}

bool ball_intersects_paddle(const Ball& ball, const Paddle& paddle) {
    const Circle& c1 = ball.getCircle();
    const Circle& c2 = paddle.getCircle();
    double dx = c1.center.x - c2.center.x;
    double dy = c1.center.y - c2.center.y;
    double distance_sq = dx * dx + dy * dy;
    double radius_sum = c1.r + c2.r;
    return distance_sq < radius_sum * radius_sum;
}

bool brick_intersects_paddle(const Brick& brick, const Paddle& paddle) {
    const Square& s = brick.getSquare();
    const Circle& c = paddle.getCircle();
    double half = s.side / 2.0;
    double left = s.center.x - half;
    double right = s.center.x + half;
    double bottom = s.center.y - half;
    double top = s.center.y + half;
    
    double closestX = max(left, min(c.center.x, right));
    double closestY = max(bottom, min(c.center.y, top));
    
    double dx = c.center.x - closestX;
    double dy = c.center.y - closestY;
    
    return (dx*dx + dy*dy) < (c.r * c.r);
}