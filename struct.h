#pragma once

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
} MandelbrotState;

typedef struct AppSettings 
{
	int unsaved_changes;
    palette* palette_list;
    palette pa;
    int nbRepeat;
    double scrollSpeed;
    int maxIt;
} AppSettings;

typedef struct OverallState
{
    AppSettings* settings;
    MandelbrotState* state;
} OverallState;
