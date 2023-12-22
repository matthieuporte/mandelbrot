#include <gtk/gtk.h>
#include "struct.h"

void mandelbrot(cairo_t* cr, point* co, size_t a, MandelbrotState* b,
        AppSettings* c, int w, int h);
