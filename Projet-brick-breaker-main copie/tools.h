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

void draw_circle(const Cairo::RefPtr<Cairo::Context>& cr, const Circle& c);
void draw_square(const Cairo::RefPtr<Cairo::Context>& cr, const Square& s);
void draw_arena(const Cairo::RefPtr<Cairo::Context>& cr);

#endif