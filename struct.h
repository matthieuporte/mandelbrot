#pragma once
#include <gtk/gtk.h>

typedef struct point {
	int x;
	int y;
}point;

typedef struct palette {
	int n;
    char* title;
	int* colors; // r1 ; g1 ; b1 ; ... ; rn ; gn ; bn 
}palette;

typedef struct MandelbrotState 
{
    double startReal;
    double startIm;
    double zoom;
    int* pixBuf;
    int w;
    int h;
} MandelbrotState;

typedef struct AppSettings 
{
	int unsaved_changes;
    palette* palette_list;
    palette palette;
    int nbRepeat;
    double scrollSpeed;
    int maxIt;
    int nbStep;
    int nbThreads;
} AppSettings;

typedef struct OverallState
{
    AppSettings* settings;
    MandelbrotState* state;
} OverallState;


typedef struct thread_data{
	cairo_t* cr;
    size_t size;
	point* coordinates;
    OverallState* os;
}thread_data;
