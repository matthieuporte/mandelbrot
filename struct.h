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
    double scroll;
    int* pixBuf;
    GdkPixbuf* colorBuf;
    int curStep;
    int w;
    int h;
    gboolean panelHidden;
} MandelbrotState;

typedef struct AppSettings 
{
	int unsaved_changes;
    palette* palette_list;
    palette palette;
    int nbRepeat;
    double scrollSpeed;
    int maxIt;
    int nbThreads;
    int nbSteps;
} AppSettings;

typedef struct RenderInfo
{
    gboolean init_done;
    guint id;
    point* coordinates;
    int curStep;
    int size;
    int remSize;
} RenderInfo;

typedef struct OverallState
{
    AppSettings* settings;
    MandelbrotState* state;
    RenderInfo* renderInfo;
    GtkDrawingArea* area;
    GtkBox* settings_panel;
} OverallState;


typedef struct thread_data{
	guchar* pix;
    size_t size;
	point* coordinates;
    OverallState* os;
}thread_data;
