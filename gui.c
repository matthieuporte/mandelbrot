#include "src/glad/glad.h"
#include <GL/gl.h>
#include <gtk/gtk.h>


// Signal handler for the "clicked" signal of the start button.
void
hello(GtkMenuItem *item, gpointer user_data)
{
    g_print("Hello World\n");
}

int
gui_run (void)
{

    gtk_init(NULL,NULL);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "./glade/app.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkGLArea* area = GTK_GL_AREA(gtk_builder_get_object(builder, "area"));
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.mandelbrot"));

    GtkMenuBar *menubar = GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar"));
    GtkMenuItem *fileMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_file"));
    GtkMenuItem *btn_new = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_new"));
    GtkMenu *submenu_file = GTK_MENU(gtk_builder_get_object(builder, "submenu_file"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(btn_new), "activate", G_CALLBACK(hello), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));

    gtk_main();

    return 0;
}
