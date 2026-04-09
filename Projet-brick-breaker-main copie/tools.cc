#include "tools.h"
#include "graphic_gui.h"
#include <cmath>
#include <algorithm>
#include "Constants.h"

extern const Cairo::RefPtr<Cairo::Context>* ptcr;

//collision cercle contre cercle
bool circle_intersects_circle(const Circle& c1, const Circle& c2)
{
    double dx = c1.center.x - c2.center.x;
    double dy = c1.center.y - c2.center.y;
    double distance_sq = dx * dx + dy * dy;
    double radius_sum = c1.r + c2.r;

    return distance_sq < radius_sum * radius_sum;
}

//collision carre contre carre
bool square_intersects_square(const Square& s1, const Square& s2)
{
    double dx = std::abs(s1.center.x - s2.center.x);
    double dy = std::abs(s1.center.y - s2.center.y);
    double limit = (s1.side + s2.side) / 2.0;

    return (dx < limit && dy < limit);
}

//collision cercle contre carre
bool circle_intersects_square(const Circle& c, const Square& s)
{
    double half = s.side / 2.0;

    double left   = s.center.x - half;
    double right  = s.center.x + half;
    double bottom = s.center.y - half;
    double top    = s.center.y + half;

    double closestX = std::max(left,  std::min(c.center.x, right));
    double closestY = std::max(bottom, std::min(c.center.y, top));

    double dx = c.center.x - closestX;
    double dy = c.center.y - closestY;

    return (dx * dx + dy * dy) < (c.r * c.r);
}

//collision carre contre cercle
bool square_intersects_circle(const Square& s, const Circle& c)
{
    return circle_intersects_square(c, s);
}

// Dessine un cercle
void draw_circle (const Circle& c)
{
    (*ptcr)->arc(c.center.x, c.center.y, c.r, 0.0, 2.0 * M_PI);
    (*ptcr)->fill();
}

// Dessine un carré 
void draw_square (const Square& s)
{
    double half = s.side / 2.0;
    (*ptcr)->rectangle(s.center.x - half, s.center.y - half, s.side, s.side);
    (*ptcr)->fill();
}

// Dessine l'arène 
void draw_arena ()
{
    (*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
    (*ptcr)->set_line_width(2.0);
    (*ptcr)->rectangle(0, 0, arena_size, arena_size);
    (*ptcr)->stroke();
}