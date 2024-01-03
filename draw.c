#include <pthread.h>
#include <gtk/gtk.h>
#include "mandelbrot.h"
#include <err.h>
#include "struct.h"

void *worker(void* arg){	
	thread_data* d = (thread_data *)arg;
    mandelbrot(d->cr,d->coordinates,d->size,d->os->state,d->os->settings);
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
    state->w = w;
    state->h = h;


    int nbThreads = os->settings->nbThreads;
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

	coorShuff(coordinates,nbPix);
	pthread_t thr[nbThreads];
	thread_data data[nbThreads];

    for (int j = 0; j < nbThreads; j++){
        data[j].cr = cr;
        data[j].coordinates = coordinates+j*size;
        data[j].os = os;
        data[j].size = (size_t)size;
        if (j == nbThreads - 1)
            data[j].size += (size_t)remSize;
        int e = pthread_create(thr+j, NULL, worker, (void *)(data+j));
        if (e != 0)
            errx(EXIT_FAILURE, "pthread_create()");
        g_print("%d\n",j);
    }
    for (int j = 0; j < nbThreads; j++){
        g_print("%d\n",j);
        pthread_join(thr[j],NULL);
    }	
    cairo_fill(cr);


	return FALSE;
}
