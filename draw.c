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

    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_width(widget);

    state->colorBuf = gdk_pixbuf_scale_simple(state->colorBuf,
            w, h, GDK_INTERP_BILINEAR);

    GdkPixbuf* pixbuf = state->colorBuf;
    state->w = w;
    state->h = h;

    int nbSteps = os->settings->nbSteps;
    int nbThreads = os->settings->nbThreads;
    int nbPix = w * h;

    point* coordinates = malloc(nbPix*sizeof(point));
	int size = nbPix/(nbThreads*nbSteps);
	int remSize = nbPix%(nbThreads*nbSteps);

	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			point p;
			p.x = x;
			p.y = y;
			coordinates[y*w + x] = p;
		}
	}

    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

	coorShuff(coordinates,nbPix);
	GThread* thr[nbThreads];
	thread_data data[nbThreads*nbSteps];

    for (int i = 0; i < nbSteps; i++){
        for (int j = 0; j < nbThreads; j++){
            int index = i*nbThreads + j;
            data[index].pix = pixels;
            data[index].coordinates = coordinates+j*size;
            data[index].os = os;
            data[index].size = (size_t)size;
            if (index == nbThreads*nbSteps - 1)
                data[j].size += (size_t)remSize;
            thr[j] = g_thread_new("mythread",worker,data+index);
        }
        for (int j = 0; j < nbThreads; j++){
            g_thread_join(thr[j]);
        }
        /* cairo_fill(cr); */
        GdkPixbuf *modifiedPixbuf = gdk_pixbuf_copy(pixbuf);

        gdk_cairo_set_source_pixbuf(cr, modifiedPixbuf, 0, 0);
        cairo_paint(cr);

        g_object_unref(modifiedPixbuf);
        /* gtk_widget_queue_draw(os->area); */
    }

	return FALSE;
}
