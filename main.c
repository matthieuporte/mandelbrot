#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <err.h>
#include "mandelbrot.h"

const int INIT_WIDTH = 1000;
const int INIT_HEIGHT = 1000;


void draw(SDL_Renderer* renderer, SDL_Surface* surface, double real,
		double im, double zoom, int maxIt)
{

	SDL_LockSurface(surface);
    // Draws the fractal canopy.
	mandelbrot(surface, real, im, zoom, maxIt);
	SDL_UnlockSurface(surface);


	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
    // Updates the display.
    SDL_RenderPresent(renderer);
}

void smooth_draw(SDL_Renderer* renderer, SDL_Surface* surface, double real, double im,
													double zoom, int maxIt){
	for (size_t i = maxIt/3; i < maxIt; i += 10){
		draw(renderer, surface, real, im ,zoom, i);
	}
}

void event_loop(SDL_Renderer* renderer, SDL_Surface* surface)
{
    // Width and height of the window.
    int w = INIT_WIDTH;
    int h = INIT_HEIGHT;
	int x;
	int y;
	double real = -2;
	double im = 1.5;
	double zoom = 3;
	int maxIt = 300;
	int curIt = maxIt;
	int discretion = maxIt/2;
	double scrollSpeed = 6;
	double scroll = 0.5;
	

    // Draws the fractal canopy (first draw).
    smooth_draw(renderer, surface, real, im, zoom, maxIt);

    // Creates a variable to get the events.
    SDL_Event event;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    w = event.window.data1;
                    h = event.window.data2;
					curIt = discretion;
    				draw(renderer, surface, real, im, zoom, curIt);
                }
                break;

			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0){
					SDL_GetMouseState(&x, &y);
					real += (double)x/w*scroll;
					im -= (double)y/h*scroll;
					zoom -= scroll;
					scroll = zoom/scrollSpeed;
					curIt = discretion;
				}
				else if (event.wheel.y < 0){
					SDL_GetMouseState(&x, &y);
					real -= (double)x/w*scroll;
					im += (double)y/h*scroll;
					zoom += scroll;
					scroll = zoom/scrollSpeed;
					curIt = discretion;
				}
    			draw(renderer, surface, real, im, zoom, curIt);
				break;
			default:
				if (curIt < maxIt){
					curIt += discretion;
    				smooth_draw(renderer, surface, real, im, zoom, curIt);
				}
        }
    }
}


int main(int argc, char *argv[]){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Mandelbrot explorer",0 ,0,
			INIT_WIDTH, INIT_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (window == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
													SDL_RENDERER_ACCELERATED);

	SDL_Surface* surface  = SDL_CreateRGBSurface(0, INIT_WIDTH, INIT_HEIGHT,
												32, 0,0,0,0);

	if (renderer == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	event_loop(renderer,surface);

	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
