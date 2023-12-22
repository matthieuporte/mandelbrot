
#include <gtk/gtk.h>
#include "mandelbrot.h"
#include "struct.h"

void coorShuff (point* coord,int n){

	srand((unsigned int)time(NULL));

	for (int i = 0; i < n; i++){
		point p = coord[i];
		int r = rand() % n;
		coord[i] = coord[r];
		coord[r] = p;
	}
}

// Signal handler for the "draw" signal of the drawing area.
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    g_print("on draw()\n");

    OverallState* os = user_data;
    MandelbrotState* state = os->state;
    AppSettings* settings = os->settings;
    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_width(widget);

    int nbPix = w * h;
    point* coordinates = malloc(nbPix*sizeof(point));

	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			point p;
			p.x = x;
			p.y = y;
			coordinates[y*w + x] = p;
		}
	}

	coorShuff(coordinates,nbPix);

    mandelbrot(cr,coordinates,nbPix,state,settings,w,h);
    cairo_fill(cr);

	return FALSE;
}
