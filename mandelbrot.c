#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <err.h>
#include <math.h>
#include "mandelbrot.h"

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


void mandelbrot(SDL_Surface* surface,point* coordinates,size_t n_coord,
		double startReal,	double startIm, double zoom, int maxIt){
	Uint32* pixels = (Uint32*)surface->pixels;

	int sizePalette = 6;
	int nbRepeat = 4;

	int r[] = {0,1,1,1,1,1};
	int g[] = {0,0,1,1,1,0};
	int b[] = {0,0,0,1,0,0};

	/* int r[] = {0,0,1}; */
	/* int g[] = {0,0,1}; */
	/* int b[] = {0,1,1}; */

	/* int r[] = {1,0,0,1}; */
	/* int g[] = {1,0,0,0}; */
	/* int b[] = {1,1,0,1}; */

	double w = surface->w;
	double h = surface->h;

	double nudge_x = zoom/w;
	double nudge_y = zoom/h;


	for (size_t p = 0; p < n_coord; p++){
		int y = coordinates[p].y;
		int x = coordinates[p].x;
		int i = (int)y * (int)w + (int)x;
		double real = startReal + x*nudge_x;
		double im = startIm - y*nudge_y;
		if (optim1(real,im)){
			pixels[i] = SDL_MapRGB(surface->format, 0, 0, 0);
		} else {
			int n = divergence(real,im,maxIt);
			if (n == -1)
				pixels[i] = SDL_MapRGB(surface->format, 0, 0, 0);
			else {
				int bloc = maxIt/(sizePalette*nbRepeat);
				/* printf("bloc = %d\n",bloc); */
				int dist = (n%bloc)*255/bloc;
				/* printf("dist = %d\n",dist); */
				int p1 = (n/bloc)%sizePalette;
				int p2 = (p1 + 1)%sizePalette;
				pixels[i] = SDL_MapRGB(surface->format, 
						r[p2]*dist + r[p1]*(255 - dist),
						g[p2]*dist + g[p1]*(255 - dist),
						b[p2]*dist + b[p1]*(255 - dist)); 
			}
		}
	}

}
