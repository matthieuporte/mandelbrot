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

    /* GtkMenuBar *menubar = GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar")); */
    /* GtkMenuItem *fileMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_file")); */
    GtkMenuItem *btn_file_new = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_new"));
    GtkMenuItem *btn_file_quit = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_quit"));
    /* GtkMenu *submenu_file = GTK_MENU(gtk_builder_get_object(builder, "submenu_file")); */

	// Creating GTK Widgets for window : UNSAVED_CHANGES_POPUP
	GtkWindow* unsaved_changes_popup_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.unsaved_changes"));
	GtkButton* unsaved_changes_quit = GTK_BUTTON(gtk_builder_get_object(builder, "unsaved_changes_quit"));
	GtkButton* unsaved_changes_save = GTK_BUTTON(gtk_builder_get_object(builder, "unsaved_changes_save"));

	
	
	/* GtkWindow** windows = malloc(sizeof(GtkWindow*)*2); // Stores references to all the app's windows; */
	/* windows = { */
	/* 	main_window, */ 
	/* 	unsaved_changes_popup_window, */
	/* }; */

	/* os->windows = windows; */

	// Connecting signal handlers
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_quit), NULL);
	g_signal_connect(area, "draw", G_CALLBACK(on_draw), os);
    g_signal_connect(G_OBJECT(btn_file_new), "activate", G_CALLBACK(hello), NULL);
    g_signal_connect(G_OBJECT(btn_file_quit), "activate", G_CALLBACK(on_quit), os);

    gtk_widget_show_all(GTK_WIDGET(main_window));

    gtk_main();

    return 0;
}
