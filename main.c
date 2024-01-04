#include "gui.h"
#include "palette.h"
#include <stdlib.h>

// Application entry point
int
main (int argc, char **argv)
{

	// Initializing Application state
	MandelbrotState* mandstate = malloc(sizeof(MandelbrotState));
	mandstate->startReal = -2;
    mandstate->w = 100;
    mandstate->h = 100;
	mandstate->startIm = 1;
    mandstate->zoom = 3;
    mandstate->scroll = 2;
    mandstate->colorBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 960, 540);
	
	AppSettings* appset = malloc(sizeof(AppSettings));
	appset->unsaved_changes = 1;
    appset->palette_list = load_palettes();
    appset->palette = appset->palette_list[0];
    appset->nbRepeat = 3;
    appset->scrollSpeed = 1.5;
    appset->maxIt = 300;
    appset->nbThreads = 16;
    appset->nbSteps = 10;

    OverallState* os = malloc(sizeof(OverallState));
    os->state = mandstate;
    os->settings = appset;

	return gui_run(&argc, &argv, os);
}
