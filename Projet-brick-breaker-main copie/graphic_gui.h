#ifndef GRAPHIC_GUI_H
#define GRAPHIC_GUI_H

#include "graphic.h"
#include <gtkmm/drawingarea.h>

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr);
void set_color(double r, double g, double b);

#endif
