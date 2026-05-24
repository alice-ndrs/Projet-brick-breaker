#ifndef TOOLS_H
#define TOOLS_H
#include "Constants.h"

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

// --- Surcharges d'opérateurs ---
Point operator-(const Point &p1, const Point &p2);
Point operator+(const Point &p1, const Point &p2);
Point operator*(double a, const Point &p);

double dot(const Point &p1, const Point &p2);
double squared_norm(const Point &p);

// --- Fonctions de collisions ---
bool circle_intersects_circle(const Circle &c1, const Circle &c2, bool epsil = false);
bool square_intersects_square(const Square &s1, const Square &s2, bool epsil = false);
bool circle_intersects_square(const Circle &c, const Square &s, bool epsil = false);
bool square_intersects_circle(const Square &s, const Circle &c, bool epsil = false);

#endif