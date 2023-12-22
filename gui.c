#include <gtk/gtk.h>
#include "mandelbrot.h"

struct MandelbrotState 
{
    double startReal;
    double startIm;
    double zoom;
    int maxIt;
};

struct AppSettings 
{
	int unsaved_changes;
    palette pa;
    double scrollSpeed;
};


// Signal handler for the "clicked" signal of the start button.
void hello(GtkMenuItem *item, gpointer user_data)
{
    g_print("Hello World\n");
}


// Signal handler for any action that would close the program.
gboolean on_quit(GtkMenuItem* item, gpointer user_data)
{
	struct AppSettings* appset = userdata;

	if (appset->unsaved_changes)
		g_print("You have unsaved changes");
}

// Signal handler for the "draw" signal of the drawing area.
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    g_print("on draw()\n");

    int w = gtk_widget_get_allocated_width(widget);
    int h = gtk_widget_get_allocated_width(widget);

    mandelbrot(cr,coords,pa,n_coord,state,w,h);
    cairo_fill(cr);

	return FALSE;
}

// Main function.
int gui_run (int* argc, char** argv[])
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

	// Creating GTK Widgets
    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.mandelbrot"));

    GtkMenuBar *menubar = GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar"));
    GtkMenuItem *fileMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_file"));
    GtkMenuItem *btn_file_new = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_new"));
    GtkMenuItem *btn_file_quit = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_quit"));
    GtkMenu *submenu_file = GTK_MENU(gtk_builder_get_object(builder, "submenu_file"));

	// Initializing Application state
	struct MandelbrotState* mandstate = malloc(sizeof(struct MandelbrotState));
	mandstate->x = 0;
	mandstate->x = 0;
	
	struct AppSettings* appset = malloc(sizeof(struct AppSettings));
	appset->unsaved_changes = 0;

	// Connecting signal handlers
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(area, "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(G_OBJECT(btn_file_new), "activate", G_CALLBACK(hello), NULL);
    g_signal_connect(G_OBJECT(btn_file_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));

    gtk_main();

    return 0;
}
