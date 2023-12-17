#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <time.h>
#include <pthread.h>
#include "mandelbrot.h"
#include "linked.h"

#define INIT_WIDTH 1000
#define INIT_HEIGHT 1000

typedef struct thread_data{
	SDL_Surface* surface;
	point* coordinates;
	palette p;
	size_t size;
	double real;
	double im;
	double zoom;
	int maxIt;
}thread_data;

void coorShuff (point coord[],int n){

	srand((unsigned int)time(NULL));

	for (int i = 0; i < n; i++){
		point p = coord[i];
		int r = rand() % n;
		coord[i] = coord[r];
		coord[r] = p;
	}
}

void *worker(void* arg){	
	thread_data* d = (thread_data *)arg;
	mandelbrot(d->surface,d->coordinates,d->p,d->size, d->real, d->im, d->zoom, d->maxIt);
	return NULL;
}

void draw(SDL_Renderer* renderer, SDL_Surface* surface,palette p, double real,
		double im, double zoom,int maxIt)
{

	if (maxIt == -1){

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_RenderCopy(renderer, texture, NULL, NULL);
		// Updates the display.
		SDL_RenderPresent(renderer);
		return;
	}

	int nbPix = INIT_WIDTH * INIT_HEIGHT;
	int nbStep = 10;
	int nbThreads = 16;

	int size = nbPix/(nbStep*nbThreads);
	int remSize = nbPix%(nbStep*nbThreads);

	point coordinates[nbPix];

	for (int y = 0; y < INIT_HEIGHT; y++){
		for (int x = 0; x < INIT_WIDTH; x++){
			point p;
			p.x = x;
			p.y = y;
			coordinates[y*INIT_WIDTH + x] = p;
		}
	}

	coorShuff(coordinates,nbPix);

	pthread_t thr[nbThreads];
	thread_data data[nbThreads*nbStep];

	for (int i = 0; i< nbStep; i++){
		SDL_LockSurface(surface);
		for (int j = 0; j < nbThreads; j++){
			int ind = i * nbThreads + j;
			data[ind].surface = surface;
			data[ind].coordinates = coordinates+(i*nbThreads+j)*size;
			data[ind].p = p;
			data[ind].size = (size_t)size;
			if (i == nbStep - 1)
				data[ind].size += (size_t)remSize;
			data[ind].real = real;
			data[ind].im = im;
			data[ind].zoom = zoom;
			data[ind].maxIt = maxIt;
			int e = pthread_create(thr+j, NULL, worker, (void *)(data+ind));
			if (e != 0)
				errx(EXIT_FAILURE, "pthread_create()");
		}
		for (int j = 0; j < nbThreads; j++){
			pthread_join(thr[j],NULL);
		}	
		SDL_UnlockSurface(surface);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_RenderCopy(renderer, texture, NULL, NULL);
		// Updates the display.
		SDL_RenderPresent(renderer);
	}
}


void event_loop(SDL_Renderer* renderer, Node* head,palette p)
{
    // Width and height of the window.
    int w = INIT_WIDTH;
    int h = INIT_HEIGHT;
	int x;
	int y;
	double real = head->real;
	double im = head->im;
	double zoom = head->zoom;
	int maxIt = 1000;
	double scrollSpeed = 1.5;
	double scroll = head->scroll;
	Node* curNode = head;

	SDL_Surface* surface = head->data;

    // Draws the fractal canopy (first draw).
    draw(renderer, surface,p, real, im, zoom, maxIt);

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
    				draw(renderer, curNode->data,p, real, im, zoom, maxIt);
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
					insert(&head,newSurface,real,im,zoom,scroll);
					curNode = curNode->next;
					draw(renderer, newSurface,p, real, im, zoom, maxIt);
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
						deleteNode(&head,curNode);
						curNode = p;
						real = curNode->real;
						im = curNode->im;
						zoom = curNode->zoom;
						scroll = curNode->scroll;
					}
					draw(renderer, curNode->data,p, real, im, zoom, -1);
				}
				break;
        }
    }
}

int ctoi (char c){
	if (c <= 'F' && c >= 'A')
		return c - 'A' + 10;
	if (c <= 'f' && c >= 'a')
		return c - 'a' + 10;
	if (c <= '9' && c >= '0')
		return c - '0';
	errx(EXIT_FAILURE,"Not a valid hex");
}

palette parse(char* filename){
	palette p;

    p.colors = malloc(3*sizeof(int));

    int n = 0;
    FILE *fptr;

    fptr = fopen(filename, "r");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fptr)) != -1){
        if (line[7] != '\n'){
            printf("len is %zu :%c;\n",len,line[7]);
            errx(EXIT_FAILURE, "Malformated palette file (hex size)");
        }
        n++;
        p.colors = realloc(p.colors,3*n*sizeof(int));
        p.colors[(n-1)*3] = ctoi(line[1])*16 + ctoi(line[2]);// red
        p.colors[(n-1)*3+1] = ctoi(line[3])*16 + ctoi(line[4]);// green
        p.colors[(n-1)*3+2] = ctoi(line[5])*16 + ctoi(line[6]);// blue
    }

    if (n == 0)
        errx(EXIT_FAILURE, "The palette must have one color");

    fclose(fptr);
    free(line);

    p.n = n;

	return p;

}

int main(int argc, char *argv[]){
	palette p;
	if (argc > 2){
		printf("Use : %s (./path_to_your_theme)\n",argv[0]);
		return 0;
	}
	else if (argc == 2){
        p = parse(argv[1]);
	}
	else {
		p.n = 2;
		p.colors = malloc(6*sizeof(int));
		p.colors[0] = 0;
		p.colors[1] = 0;
		p.colors[2] = 20;
		p.colors[3] = 255;
		p.colors[4] = 255;
		p.colors[5] = 255;
	}

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


	event_loop(renderer,head,p);

	while (head != NULL){
		SDL_FreeSurface(head->data);
		Node* prev = head;
		head = head->next;
		free(prev);
	}

	free(p.colors);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
