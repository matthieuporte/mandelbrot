#include <gtk/gtk.h>
#include "struct.h"

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);

gboolean render_step(gpointer user_data);
void hd_render(GdkPixbuf* pixbuf, OverallState* os);
