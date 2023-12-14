
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <err.h>

typedef struct point {
	int x;
	int y;
}point;

void mandelbrot(SDL_Surface* surface, point* co, size_t n, double a, double b, double c,int d);
