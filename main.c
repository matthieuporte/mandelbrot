#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "mandelbrot.h"
#include "linked.h"

const int INIT_WIDTH = 1000;
const int INIT_HEIGHT = 1000;


void draw(SDL_Renderer* renderer, SDL_Surface* surface, double real,
		double im, double zoom, int curIt,int maxIt)
{

	if (maxIt != -1){
		SDL_LockSurface(surface);
    	// Draws the fractal canopy.
		mandelbrot(surface, real, im, zoom, curIt, maxIt);
		SDL_UnlockSurface(surface);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
    // Updates the display.
    SDL_RenderPresent(renderer);
}

void smooth_draw(SDL_Renderer* renderer, SDL_Surface* surface, double real, double im,
													double zoom, int maxIt){
	for (size_t i = 290; i < maxIt; i += 10){
		draw(renderer, surface, real, im ,zoom, i, maxIt);
	}
}

void event_loop(SDL_Renderer* renderer, Node** head)
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
	double scrollSpeed = 1.5;
	double scroll = 2;
	Node* curNode = *head;

	SDL_Surface* surface = (*head)->data;

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
    				smooth_draw(renderer, curNode->data, real, im, zoom, maxIt);
                }
                break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT){
					SDL_GetMouseState(&x, &y);
					real += (double)x/w*scroll;
					im -= (double)y/h*scroll;
					zoom -= scroll;
					scroll = zoom/scrollSpeed;

					SDL_Surface* newSurface = SDL_CreateRGBSurface(0, INIT_WIDTH,
									INIT_HEIGHT,32, 0,0,0,0);
					insert(head,newSurface,real,im,zoom,scroll);
					curNode = curNode->next;
					smooth_draw(renderer, newSurface, real, im, zoom, maxIt);
				}
				if (event.button.button == SDL_BUTTON_RIGHT){
					/* SDL_GetMouseState(&x, &y); */
					/* real -= (double)x/w*scroll; */
					/* im += (double)y/h*scroll; */
					/* zoom += scroll; */
					/* scroll = zoom/scrollSpeed; */
					
					if (curNode->prev != NULL){
						SDL_FreeSurface(curNode->data);
						Node* p = curNode->prev;
						deleteNode(head,curNode);
						curNode = p;
						real = curNode->real;
						im = curNode->im;
						zoom = curNode->zoom;
						scroll = curNode->scroll;
					}
					draw(renderer, curNode->data, real, im, zoom,-1, -1);
				}
				break;
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

	Node* head = NULL;
	SDL_Surface* surface = SDL_CreateRGBSurface(0, INIT_WIDTH, INIT_HEIGHT,
												32, 0,0,0,0);

	insert(&head,surface,-2,1.5,3,2);

	if (renderer == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	event_loop(renderer,&head);

	while (head->prev != NULL)
		head = head->prev;

	while (head->next != NULL){
		SDL_FreeSurface(head->data);
		Node* prev = head;
		head = head->next;
		free(prev);
	}
	free(head);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
