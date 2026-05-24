#include "tools.h"
#include <algorithm>
#include <cmath>

// ------- Surcharges d'opérateurs pour les points -------

Point operator-(const Point &p1, const Point &p2) {
    return {p1.x - p2.x, p1.y - p2.y};
}

Point operator+(const Point &p1, const Point &p2) {
    return {p1.x + p2.x, p1.y + p2.y};
}

Point operator*(double a, const Point &p) { return {a * p.x, a * p.y}; }

double dot(const Point &p1, const Point &p2) { return p1.x * p2.x + p1.y * p2.y; }

double squared_norm(const Point &p) { return dot(p, p); }

// ------- Fonctions de collisions -------

// collision cercle contre cercle
bool circle_intersects_circle(const Circle &c1, const Circle &c2, bool epsil) {
    const double tol = epsil ? epsil_zero : 0.0;

    const Point d = c1.center - c2.center;
    const double radius_sum = c1.r + c2.r + tol;

    return squared_norm(d) < radius_sum * radius_sum;
}

// collision carre contre carre
bool square_intersects_square(const Square &s1, const Square &s2, bool epsil) {
    const double tol = epsil ? epsil_zero : 0.0;

    const double dx = std::abs(s1.center.x - s2.center.x);
    const double dy = std::abs(s1.center.y - s2.center.y);
    const double limit = (s1.side + s2.side) / 2.0;

    return dx < limit + tol && dy < limit + tol;
}

// collision cercle contre carre
bool circle_intersects_square(const Circle &c, const Square &s, bool epsil) {
    const double half = s.side / 2.0;

    const double left = s.center.x - half;
    const double right = s.center.x + half;
    const double bottom = s.center.y - half;
    const double top = s.center.y + half;

    const double closestX = std::max(left, std::min(c.center.x, right));
    const double closestY = std::max(bottom, std::min(c.center.y, top));

    const Point d = c.center - Point{closestX, closestY};
    const double tol = epsil ? epsil_zero : 0.0;
    const double radius = c.r + tol;

    return squared_norm(d) < radius * radius;
}

// collision carre contre cercle
bool square_intersects_circle(const Square &s, const Circle &c, bool epsil) {
    return circle_intersects_square(c, s, epsil);
}