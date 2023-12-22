
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <err.h>

typedef struct point {
	int x;
	int y;
}point;

typedef struct palette {
	int n;
	int* colors;
}palette;

struct MandelbrotState 
{
    double startReal;
    double startIm;
    double zoom;
    int maxIt;
};

struct AppSettings 
{
	int unsaved_changes;
    palette pa;
    double scrollSpeed;
};

void mandelbrot(SDL_Surface* surface, point* co, palette pa,
		size_t n, double a, double b, double c,int d);
