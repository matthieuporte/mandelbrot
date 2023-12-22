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
	mandstate->startIm = 1.5;
    mandstate->zoom = 3;
	
	AppSettings* appset = malloc(sizeof(AppSettings));
	appset->unsaved_changes = 1;
    appset->palette_list = load_palettes();
    appset->palette = appset->palette_list[0];
    appset->nbRepeat = 1;
    appset->scrollSpeed = 1.5;
    appset->maxIt = 100;

    OverallState* os = malloc(sizeof(OverallState));
    os->state = mandstate;
    os->settings = appset;

	return gui_run(&argc, &argv, os);
}
