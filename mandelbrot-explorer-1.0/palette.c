#include <gtk/gtk.h>
#include <err.h>
#include "struct.h"

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

    if ((read = getline(&line, &len, fptr)) != -1 || strlen(line) == 0){
        errx(EXIT_FAILURE, "No title in palette");
    }

    asprintf(&p.title,"%s",line);

    while ((read = getline(&line, &len, fptr)) != -1){
        if (line[7] != '\n'){
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

palette create_palette(size_t n, char* title ,int values[n]){
    if (n%3 != 0)
        err(1,"create_palette()");
    palette p;
    p.n = n/3;
    p.title = title;
    p.colors = malloc(n*sizeof(int));
    for (size_t i = 0; i < n; i++){
        p.colors[i] = values[i];
    }
    return p;
}

palette* load_palettes(){
    int n = 8;
    palette* l = malloc(n*sizeof(palette));

    int midnight[2*3] = {0,0,15,255,255,255};
    l[0] = create_palette(2*3,"midnight",midnight);

    int fire[6*3] = {0,0,0, 255,0,0, 255,255,0, 255,255,255,
                  255,255,0, 255,0,0};
    l[1] = create_palette(6*3,"fire",fire);

    int aether[4*3] = {255,255,255, 0,0,255, 0,0,0, 255,0,255};
    l[2] = create_palette(4*3, "aether",aether);

    int blue[3*3] = {0,0,0,0,0,255,255,255,255};
    l[3] = create_palette(3*3,"blue",blue);
    
    int rainbow[7*3] = {255,0,0, 255,153,0, 255,255,0, 0,255,0, 0,255,255, 0,0,255, 255,0,255};
    l[4] = create_palette(7*3, "rainbow",rainbow);

    int seashore[6*3] = {202,255,195, 229,230,167, 241,81,32, 132,28,23, 5,116,174, 137,211,209};
    l[5] = create_palette(6*3, "seashore",seashore);

    int pastel[5*3] = {206,208,210, 112,134,255, 255,92,148, 255,255,133, 140,238,145};
    l[6] = create_palette(5*3, "pastel",pastel);

    int grayscale[2*3] = {0,0,0,255,255,255};
    l[7] = create_palette(2*3, "grayscale", grayscale);

    return l;
}
