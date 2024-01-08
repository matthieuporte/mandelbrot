#include <gtk/gtk.h>
#include "struct.h"

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);

gboolean render_step(gpointer user_data);
void custom_render(GdkPixbuf* pixbuf,int w,int h, OverallState* os);
