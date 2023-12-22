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
    palette* l = malloc(1*sizeof(palette));

    int blue[6] = {0,0,20,255,255,255};
    l[0] = create_palette(6,"blue",blue);

    return l;
}
