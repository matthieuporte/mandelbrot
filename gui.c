#include <gtk/gtk.h>
#include "mandelbrot.h"
#include "draw.h"
#include "struct.h"
#include "palette.h"

// Signal handler for the "clicked" signal of the start button.
void hello(GtkMenuItem *item, gpointer user_data)
{
    g_print("Hello World\n");
}

void redraw(OverallState* os){
    g_source_remove(os->renderInfo->id);
    os->renderInfo->init_done = FALSE;
    os->renderInfo->id = g_idle_add(render_step,os);
    gtk_widget_queue_draw(GTK_WIDGET(os->area));
}

void recalibrate(OverallState* os,int direction,double x,double y){ 
    os->state->startReal += (x/os->state->w*os->state->scroll)*direction;
    os->state->startIm += (y/os->state->w*os->state->scroll)*direction*(-1);
    os->state->zoom += (os->state->scroll)*direction*(-1);
    os->state->scroll = os->state->zoom/os->settings->scrollSpeed;
}

gboolean coordinates(GtkWidget *widget,GdkEventButton *event, gpointer user_data)
{
    OverallState* os = user_data;
    if (event->button == 1)
        recalibrate(os,1,event->x,event->y);
    else if (event->button == 3)
        recalibrate(os,-1,event->x,event->y);

    redraw(os);
    return FALSE;
}


gboolean on_scroll_event(GtkWidget *widget,GdkEventScroll *event, gpointer user_data)
{
    OverallState* os = user_data;
    if (event->direction == GDK_SCROLL_UP)
        recalibrate(os,1,event->x,event->y);
    else if (event->direction == GDK_SCROLL_DOWN)
        recalibrate(os,-1,event->x,event->y);
    
    redraw(os);
    return FALSE;
}

gboolean paletteAether(GtkMenuItem *item, gpointer user_data){
    OverallState* os = user_data;
    os->settings->palette = os->settings->palette_list[2]; 
    redraw(os);
    return FALSE;
}
gboolean paletteFire(GtkMenuItem *item, gpointer user_data){
    OverallState* os = user_data;
    os->settings->palette = os->settings->palette_list[1]; 
    redraw(os);
    return FALSE;
}
gboolean paletteBlue(GtkMenuItem *item, gpointer user_data){
    OverallState* os = user_data;
    os->settings->palette = os->settings->palette_list[0]; 
    redraw(os);
    return FALSE;
}

gboolean on_resize(GtkWidget* widget,GdkEventConfigure *event, gpointer user_data){
    OverallState* os = user_data;
    redraw(os);
    return FALSE;
}

// Signal handler for any action that would close the program.
gboolean on_quit(GtkMenuItem* item, gpointer user_data)
{
	OverallState* os = user_data;

	if (os->settings->unsaved_changes == 1)
	{
		g_print("MANDELBROT_WARNING: the current file has unsaved changes\n");
		/* gtk_widget_show_all(GTK_WIDGET(os->windows[1])); */
	}
	else
		gtk_main_quit();

    return TRUE;
}


// Main function.
int gui_run (int* argc, char** argv[], OverallState* os)
{
    gtk_init(NULL, NULL);

  
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "./glade/app.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

	// Creating GTK Widgets for window : MAIN
    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));
    GtkWindow* main_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.mandelbrot"));

    os->area = area;

    /* GtkMenuBar *menubar = GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar")); */
    /* GtkMenuItem *fileMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_file")); */
    GtkMenuItem *settingsMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_settings"));
    GtkMenuItem *palettesMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_palettes"));
    GtkMenuItem *btn_aether = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_aether"));
    GtkMenuItem *btn_fire = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_fire"));
    GtkMenuItem *btn_blue = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_blue"));
    GtkMenuItem *btn_file_new = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_new"));
    GtkMenuItem *btn_file_quit = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_quit"));
    /* GtkMenu *submenu_file = GTK_MENU(gtk_builder_get_object(builder, "submenu_file")); */

	// Creating GTK Widgets for window : UNSAVED_CHANGES_POPUP
	GtkWindow* unsaved_changes_popup_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.unsaved_changes"));
	GtkButton* unsaved_changes_quit = GTK_BUTTON(gtk_builder_get_object(builder, "unsaved_changes_quit"));
	GtkButton* unsaved_changes_save = GTK_BUTTON(gtk_builder_get_object(builder, "unsaved_changes_save"));

    // TODO change the following line
    /* gtk_widget_set_size_request(area, gdk_pixbuf_get_width(os->state->colorBuf), gdk_pixbuf_get_height(os->state->colorBuf)); */
	
	
	/* GtkWindow** windows = malloc(sizeof(GtkWindow*)*2); // Stores references to all the app's windows; */
	/* windows = { */
	/* 	main_window, */ 
	/* 	unsaved_changes_popup_window, */
	/* }; */

	/* os->windows = windows; */

	// Connecting signal handlers
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_quit), NULL);
    g_signal_connect(area, "configure-event", G_CALLBACK(on_resize), os);
    gtk_widget_add_events(GTK_WIDGET(area), GDK_BUTTON_PRESS_MASK);
    g_signal_connect(area, "button_press_event", G_CALLBACK(coordinates), os);
    gtk_widget_add_events(GTK_WIDGET(area), GDK_SCROLL_MASK);
    g_signal_connect(area, "scroll-event", G_CALLBACK(on_scroll_event), os);
	g_signal_connect(area, "draw", G_CALLBACK(on_draw), os);
    
    g_signal_connect(G_OBJECT(btn_file_new), "activate", G_CALLBACK(hello), NULL);
    g_signal_connect(G_OBJECT(btn_aether), "activate", G_CALLBACK(paletteAether), os);
    g_signal_connect(G_OBJECT(btn_fire), "activate", G_CALLBACK(paletteFire), os);
    g_signal_connect(G_OBJECT(btn_blue), "activate", G_CALLBACK(paletteBlue), os);
    g_signal_connect(G_OBJECT(btn_file_quit), "activate", G_CALLBACK(on_quit), os);

    gtk_widget_show_all(GTK_WIDGET(main_window));

    gtk_main();

    return 0;
}
