#ifndef TOOLS_H
#define TOOLS_H
#include <cairomm/context.h>

constexpr double epsil_zero = 0.125;

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


bool circle_intersects_circle(const Circle& c1, const Circle& c2);
bool square_intersects_square(const Square& s1, const Square& s2);
bool circle_intersects_square(const Circle& c, const Square& s);
bool square_intersects_circle(const Square& s, const Circle& c);


#endif