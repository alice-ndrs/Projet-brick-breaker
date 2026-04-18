#include "graphic.h"
#include "tools.h"
#include "Constants.h"
#include "graphic_gui.h"
#include <cmath>
#include <cairomm/context.h>

using namespace std;

const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}
// local functions
void set_color(Color color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:
        r = 1.0;
        break;
    case ORANGE:
        r = 1.0;
        g = 0.5;
        break;
    case YELLOW:
        r = 1.0;
        g = 1.0;
        break;
    case GREEN:
        g = 1.0;
        break;
    case CYAN:
        g = 1.0;
        b = 1.0;
        break;
    case BLUE:
        b = 1.0;
        break;
    case PURPLE:
        r = 0.5;
        b = 1.0;
        break;
    case BLACK:
        r = g = b = 0.0;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case WHITE:
        r = g = b = 1.0;
        break;
    default:
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}

// Dessine un cercle
void draw_circle (const Circle& c)
{
    (*ptcr)->move_to(c.center.x, c.center.y);
    (*ptcr)->arc(c.center.x, c.center.y, c.r, 0.0, 2.0 * M_PI);
}

// Dessine un carré 
void draw_square (const Square& s)
{
    double half = s.side / 2.0;
    (*ptcr)->rectangle(s.center.x - half, s.center.y - half, s.side, s.side);
    (*ptcr)->fill();
}

// Dessine un arc
void draw_arc(const Circle& c)
{
    double dx_squared = c.r * c.r - c.center.y * c.center.y;

    if (dx_squared < 0) return;

    double dx = sqrt(dx_squared);

    double angle1 = atan2(-c.center.y, -dx);
    double angle2 = atan2(-c.center.y,  dx);

    (*ptcr)->arc(c.center.x, c.center.y, c.r, angle1, angle2);
}

// Dessine l'arène 
void draw_arena ()
{
    set_color(GREY);
    (*ptcr)->set_line_width(0.2);
    (*ptcr)->rectangle(0, 0, arena_size, arena_size);
    (*ptcr)->stroke();
}