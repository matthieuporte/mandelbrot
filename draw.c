#include <pthread.h>
#include <gtk/gtk.h>
#include "mandelbrot.h"
#include <err.h>
#include "struct.h"

void *worker(void* arg){	
	thread_data* d = (thread_data *)arg;
    mandelbrot(d->cr,d->coordinates,d->size,d->os->state,d->os->settings,d->w,d->h);
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
    g_print("on draw()\n");

    OverallState* os = user_data;
    MandelbrotState* state = os->state;
    AppSettings* settings = os->settings;
    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_width(widget);

    int nbStep = os->settings->nbStep;
    int nbThreads = os->settings->nbThreads;
    int nbPix = w * h;

    point* coordinates = malloc(nbPix*sizeof(point));
	int size = nbPix/(nbStep*nbThreads);
	int remSize = nbPix%(nbStep*nbThreads);

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
	thread_data data[nbThreads*nbStep];

	for (int i = 0; i< nbStep; i++){
		for (int j = 0; j < nbThreads; j++){
			int ind = i * nbThreads + j;
			data[ind].cr = cr;
			data[ind].coordinates = coordinates+(i*nbThreads+j)*size;
			data[ind].os = os;
			data[ind].size = (size_t)size;
            data[ind].w = w;
            data[ind].h = h;
			if (i == nbStep - 1 && j == nbThreads - 1)
				data[ind].size += (size_t)remSize;
			int e = pthread_create(thr+j, NULL, worker, (void *)(data+ind));
			if (e != 0)
				errx(EXIT_FAILURE, "pthread_create()");
		}
		for (int j = 0; j < nbThreads; j++){
			pthread_join(thr[j],NULL);
		}	
        cairo_fill(cr);
	}


	return FALSE;
}
