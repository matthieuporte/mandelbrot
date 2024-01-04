#include <pthread.h>
#include <gtk/gtk.h>
#include "mandelbrot.h"
#include <err.h>
#include "struct.h"

void *worker(void* arg){	
	thread_data* d = (thread_data *)arg;
    mandelbrot(d->pix,d->coordinates,d->size,d->os);
	return NULL;
}

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

    OverallState* os = user_data;
    MandelbrotState* state = os->state;
    GdkPixbuf* pixbuf = state->colorBuf;
    /* int w = gtk_widget_get_allocated_width(widget); */
    /* int h = gtk_widget_get_allocated_width(widget); */
    int w = gdk_pixbuf_get_width(pixbuf);
    int h = gdk_pixbuf_get_height(pixbuf);
    state->w = w;
    state->h = h;


    int nbThreads = 16;//os->settings->nbThreads;
    int nbPix = w * h;

    point* coordinates = malloc(nbPix*sizeof(point));
	int size = nbPix/nbThreads;
	int remSize = nbPix%nbThreads;

	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			point p;
			p.x = x;
			p.y = y;
			coordinates[y*w + x] = p;
		}
	}

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    // Example: Manipulate pixel data (e.g., invert colors)
    /* for (int i = 0; i < w * h * 3; i++) { */
    /*     pixels[i] = 255 - pixels[i]; */
    /* } */


	coorShuff(coordinates,nbPix);
	GThread* thr[nbThreads];
	thread_data data[nbThreads];

    for (int j = 0; j < nbThreads; j++){
        data[j].pix = pixels;
        data[j].coordinates = coordinates+j*size;
        data[j].os = os;
        data[j].size = (size_t)size;
        if (j == nbThreads - 1)
            data[j].size += (size_t)remSize;
        thr[j] = g_thread_new("mythread",worker,data+j);
    }
    for (int j = 0; j < nbThreads; j++){
        g_thread_join(thr[j]);
    }
    /* cairo_fill(cr); */
    GdkPixbuf *modifiedPixbuf = gdk_pixbuf_copy(pixbuf);

    gdk_cairo_set_source_pixbuf(cr, modifiedPixbuf, 0, 0);
    cairo_paint(cr);

    g_object_unref(modifiedPixbuf);

	return FALSE;
}
