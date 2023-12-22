#include <stdio.h>
#include <err.h>
#include <math.h>
#include <gtk/gtk.h>
#include "struct.h"

// a^2 - b^2 + 2abi
void complexSquared(double* real, double* im){
	double oldReal = *real;
	double oldIm = *im;
	*real = oldReal*oldReal - oldIm * oldIm;
	*im = 2 * oldReal * oldIm;
}

int divergence(double real, double im, int depth){
	int n;
	double myReal = 0;
	double myIm = 0;
	for (n = 0; n < depth; n++){
		complexSquared(&myReal,&myIm);
		myReal += real;
		myIm += im;
		if (myReal > 2 || myReal < -2){
			if (myIm > 2 || myIm < -2)
				return n;
		}
	}
	return -1;
}

int optim1(double x, double y){
	if ((x + 1)*(x + 1) + y*y < 0.0625){
		return 1;
	}
	double p = sqrt((x - 0.25)*(x - 0.25) + y*y);
	if (x < p - 2*p*p + 0.25){
		return 1;
	}
	return 0;
}


void mandelbrot(cairo_t* cr,point* coordinates, int n_coord,
        MandelbrotState* state, AppSettings* settings, int w, int h){

	int sizePalette = settings->pa.n;
	int nbRepeat = settings->nbRepeat;

	double nudge_x = state->zoom/w;
	double nudge_y = state->zoom/h;

	for (size_t p = 0; p < n_coord; p++){
		int y = coordinates[p].y;
		int x = coordinates[p].x;
		int i = (int)y * (int)w + (int)x;
		double real = state->startReal + x*nudge_x;
		double im = state->startIm - y*nudge_y;
		if (optim1(real,im)){
            cairo_set_source_rgb(cr,0,0,0);
		} else {
			int n = divergence(real,im,settings->maxIt);
			if (n == -1)
                cairo_set_source_rgb(cr,0,0,0);
			else {
                cairo_set_source_rgb(cr,1,1,1);
				/* int bloc = maxIt/(sizePalette*nbRepeat); */
				/* int dist = (n%bloc)*255/bloc; */
				/* int p1 = (n/bloc)%sizePalette; */
				/* int p2 = (p1 + 1)%sizePalette; */
				/* pixels[i] = SDL_MapRGB(surface->format, */
				/* 	(pa.colors[p2*3]*dist + pa.colors[p1*3]*(255 - dist))/255, */
				/* 	(pa.colors[p2*3+1]*dist + pa.colors[p1*3+1]*(255 - dist))/255, */
				/* 	(pa.colors[p2*3+2]*dist + pa.colors[p1*3+2]*(255 - dist))/255); */
			}
		}
        cairo_rectangle(cr,x,y,1,1);
        cairo_fill(cr);
	}

}
