#pragma once

typedef struct point {
	int x;
	int y;
}point;

typedef struct palette {
	int n;
	int* colors;
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
