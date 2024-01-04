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
	mandstate->startIm = 1;
    mandstate->zoom = 3;
    mandstate->scroll = 2;
    mandstate->colorBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 960, 540);
	
	AppSettings* appset = malloc(sizeof(AppSettings));
	appset->unsaved_changes = 1;
    appset->palette_list = load_palettes();
    appset->palette = appset->palette_list[0];
    appset->nbRepeat = 50;
    appset->scrollSpeed = 1.5;
    appset->maxIt = 10000;
    appset->nbThreads = 1;
    appset->nbSteps = 10;

    RenderInfo* ri = malloc(sizeof(RenderInfo));
    ri->init_done = FALSE;
    ri->coordinates = NULL;
    ri->curStep = 0;
    ri->size = 0;
    ri->remSize = 0;

    OverallState* os = malloc(sizeof(OverallState));
    os->state = mandstate;
    os->settings = appset;
    os->renderInfo = ri;

	return gui_run(&argc, &argv, os);
}
