#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "tools.h"

enum Color {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    BLACK,
    GREY,
    WHITE,
};

void set_color(Color color);
void draw_circle(const Circle &c);
void draw_square(const Square &s);
void draw_arc(const Circle &c);
void draw_arena();

#endif