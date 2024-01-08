#include <pthread.h>
#include <time.h>
#include <gtk/gtk.h>
#include "mandelbrot.h"
#include <err.h>
#include "struct.h"

void *worker(void* arg){	
	thread_data* d = (thread_data *)arg;
    mandelbrot(d->pix,d->coordinates,d->size,d->w,d->h,d->os);
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

gboolean initial_setup(OverallState* os)
{
    MandelbrotState* state = os->state;

    RenderInfo* ri = os->renderInfo;

    int w = state->w;
    int h = state->h;

    int nbSteps = os->settings->nbSteps;
    int nbThreads = os->settings->nbThreads;
    int nbPix = w * h;

    GdkPixbuf* pixbuf = state->colorBuf;
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    /* clock_t start_time = clock(); */

    if (!os->settings->transition){
        for (int i = 0; i < w * h * 3; i++) {
            pixels[i] = 0;  // Set to black
        }
    }
    
    /* clock_t end_time = clock(); */

    /* double cpu_time_used = ((double) (end_time - start_time)); */

    /* g_print("%f\n",cpu_time_used); */

    ri->coordinates = malloc(nbPix*sizeof(point));
	ri->size = nbPix/(nbThreads*nbSteps);
	ri->remSize = nbPix%(nbThreads*nbSteps);

	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			point p;
			p.x = x;
			p.y = y;
			ri->coordinates[y*w + x] = p;
		}
	}

	coorShuff(ri->coordinates,nbPix);
    ri->curStep = 0;
}

gboolean render_step(gpointer user_data){

    OverallState* os = user_data;
    MandelbrotState* state = os->state;
    RenderInfo* ri = os->renderInfo;

    if (!ri->init_done){
        initial_setup(os);
        ri->init_done = TRUE;
    }

    if (ri->curStep >= os->settings->nbSteps){
        // Cleanup and return FALSE to stop the idle callback
        ri->init_done = FALSE;
        return FALSE;
    }


    int nbSteps = os->settings->nbSteps;
    int nbThreads = os->settings->nbThreads;
    int nbPix = state->w * state->h;

	GThread* thr[nbThreads];
	thread_data data[nbThreads*nbSteps];
    GdkPixbuf* pixbuf = state->colorBuf;
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    
    for (int j = 0; j < nbThreads; j++){
        int index = ri->curStep*nbThreads + j;
        data[index].pix = pixels;
        data[index].w = state->w;
        data[index].h = state->h;
        data[index].coordinates = ri->coordinates+index*ri->size;
        data[index].os = os;
        data[index].size = (size_t)ri->size;
        if (ri->curStep == (nbSteps-1) && j == nbThreads-1){
            data[index].size += (size_t)ri->remSize;
        }
        thr[j] = g_thread_new("mythread",worker,data+index);
    }
    for (int j = 0; j < nbThreads; j++){
        g_thread_join(thr[j]);
    }

    gtk_widget_queue_draw(GTK_WIDGET(os->area));

    ri->curStep += 1;
    // Return TRUE to keep the idle callback active
    return TRUE;
}

void hd_render(GdkPixbuf* pixbuf, OverallState* os){

    int w = 1920;
    int h = 1080;

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


	GThread* thr[nbThreads];
	thread_data data[nbThreads];
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    
    for (int j = 0; j < nbThreads; j++){
        data[j].pix = pixels;
        data[j].w = w;
        data[j].h = h;
        data[j].coordinates = coordinates+j*size;
        data[j].os = os;
        data[j].size = (size_t)size;
        if (j == nbThreads-1){
            data[j].size += remSize;
        }
        thr[j] = g_thread_new("mythread",worker,data+j);
    }
    for (int j = 0; j < nbThreads; j++){
        g_thread_join(thr[j]);
    }
    free(coordinates);
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    OverallState* os = user_data;

    // Draw the current state of the Pixbuf on the drawing area
    gdk_cairo_set_source_pixbuf(cr, os->state->colorBuf, 0, 0);
    cairo_paint(cr);

    // Return FALSE to indicate that the event has been handled
    return FALSE;
}
