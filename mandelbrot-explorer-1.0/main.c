#include "gui.h"
#include "palette.h"
#include <stdlib.h>

// Application entry point
int
main (int argc, char **argv)
{

    int w = 960;
    int h = 540;

	// Initializing Application state
	MandelbrotState* mandstate = malloc(sizeof(MandelbrotState));
	mandstate->startReal = -2.5;
	mandstate->startIm = 1.1;
    mandstate->zoom = 4;
    mandstate->scroll = 2;
    mandstate->isRendering = 0;
    mandstate->colorBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
    mandstate->panelHidden = TRUE;
	
	AppSettings* appset = malloc(sizeof(AppSettings));
    appset->palette_list = load_palettes();
    appset->palette = appset->palette_list[0];
    appset->nbRepeat = 3;
    appset->scrollSpeed = 1.5;
    appset->maxIt = 1000;
    appset->nbThreads = sysconf(_SC_NPROCESSORS_ONLN);
    appset->nbSteps = 10;
    appset->transition = FALSE;

    RenderInfo* ri = malloc(sizeof(RenderInfo));
    ri->init_done = FALSE;
    ri->coordinates = NULL;
    ri->curStep = 0;
    ri->size = 0;
    ri->remSize = 0;
    ri->id = 0;

    OverallState* os = malloc(sizeof(OverallState));
    os->state = mandstate;
    os->settings = appset;
    os->renderInfo = ri;

	return gui_run(&argc, &argv, os);
}
