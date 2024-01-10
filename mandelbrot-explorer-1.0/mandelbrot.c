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


void mandelbrot(guchar* pix,point* coordinates, int n_coord,int w,int h, OverallState* os){

    MandelbrotState* state = os->state;
    AppSettings* settings = os->settings;
    palette pa = settings->palette;
	int sizePalette = pa.n;
	int nbRepeat = settings->nbRepeat;
    int maxIt = settings->maxIt;

    double nudge = state->zoom/w;

	for (int p = 0; p < n_coord; p++){
		int y = coordinates[p].y;
		int x = coordinates[p].x;
		double real = state->startReal + x*nudge;
		double im = state->startIm - y*nudge;
		if (optim1(real,im)){
            pix[(y*w + x)*3 + 0] = 0;
            pix[(y*w + x)*3 + 1] = 0;
            pix[(y*w + x)*3 + 2] = 0;
		} else {
			int n = divergence(real,im,settings->maxIt);
			if (n == -1){
                pix[(y*w + x)*3 + 0] = 0;
                pix[(y*w + x)*3 + 1] = 0;
                pix[(y*w + x)*3 + 2] = 0;
            }
			else {
				int bloc = maxIt/(sizePalette*nbRepeat);
				int dist = (n%bloc)*255/bloc;
				int p1 = (n/bloc)%sizePalette;
				int p2 = (p1 + 1)%sizePalette;
                double r = (double)(pa.colors[p2*3+0]*dist + pa.colors[p1*3+0]*(255 - dist))/255;
                double g = (double)(pa.colors[p2*3+1]*dist + pa.colors[p1*3+1]*(255 - dist))/255;
                double b = (double)(pa.colors[p2*3+2]*dist + pa.colors[p1*3+2]*(255 - dist))/255;
                pix[(y*w + x)*3 + 0] = (char)r;
                pix[(y*w + x)*3 + 1] = (char)g;
                pix[(y*w + x)*3 + 2] = (char)b;
			}
		}
	}

}
