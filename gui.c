#include <gtk/gtk.h>
#include <time.h>
#include "mandelbrot.h"
#include "draw.h"
#include "struct.h"
#include "palette.h"
#include "save.h"

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

// Signal handler for the "clicked" signal of the start button.
gboolean on_new(GtkMenuItem *item, OverallState* os)
{
	os->state->startReal = -2;
	os->state->startIm = 1;
    os->state->zoom = 3;
    os->state->scroll = 2;
    redraw(os);
    return FALSE;
}

// Signal handler for the "clicked" signal of the start button.


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
    os->settings->palette = os->settings->palette_list[3]; 
    redraw(os);
    return FALSE;
}
gboolean paletteMidnight(GtkMenuItem *item, gpointer user_data){
    OverallState* os = user_data;
    os->settings->palette = os->settings->palette_list[0]; 
    redraw(os);
    return FALSE;
}

int clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}

gboolean on_resize(GtkWidget* widget,GdkEventConfigure *event, gpointer user_data){
    OverallState* os = user_data;

    int w = event->width;
    int h = event->height;

    os->state->colorBuf = gdk_pixbuf_scale_simple(os->state->colorBuf,
            w, h, GDK_INTERP_BILINEAR);

    os->state->w = w;
    os->state->h = h;
    g_print("1 - %d|%d\n",os->state->w,os->state->h);

    redraw(os);
    return FALSE;
}

gboolean apply_settings(GtkWidget* widget, OverallState* os){
    redraw(os);
    return FALSE;
}


void switchPanel(GtkMenuItem *item, OverallState* os)
{
    if (os->state->panelHidden)
        gtk_widget_show(GTK_WIDGET(os->settings_panel));
    else
        gtk_widget_hide(GTK_WIDGET(os->settings_panel));
    os->state->panelHidden = !os->state->panelHidden;
    redraw(os);
}

void itr_scale_update(GtkRange *range, OverallState* os) {
    int value = (int)gtk_range_get_value(range);
    os->settings->maxIt = value;
    if (os->settings->nbRepeat > value/2){
        os->settings->nbRepeat = 3;
        gtk_range_set_value(GTK_RANGE(os->settings->theme_scale),3);
    }
}

void theme_scale_update(GtkRange *range, OverallState* os) {
    int value = (int)gtk_range_get_value(range);
    if (os->settings->maxIt < value*2){
        os->settings->nbRepeat = 3;
        gtk_range_set_value(range,value/2);
    }
    else
        os->settings->nbRepeat = value;
}


void threads_scale_update(GtkRange *range, OverallState* os) {
    g_source_remove(os->renderInfo->id);
    os->renderInfo->init_done = FALSE;
    int value = (int)gtk_range_get_value(range);
    os->settings->nbThreads = value;
    os->renderInfo->id = g_idle_add(render_step,os);
    gtk_widget_queue_draw(GTK_WIDGET(os->area));
}

void steps_scale_update(GtkRange *range, OverallState* os) {
    g_source_remove(os->renderInfo->id);
    os->renderInfo->init_done = FALSE;
    int value = (int)gtk_range_get_value(range);
    os->settings->nbSteps = value;
    os->renderInfo->id = g_idle_add(render_step,os);
    gtk_widget_queue_draw(GTK_WIDGET(os->area));
}

gboolean set_transition(GtkSwitch* s,gboolean state, OverallState* os){
    os->settings->transition = state;
    return FALSE;
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
    GtkBox* area_container = GTK_BOX(gtk_builder_get_object(builder, "area_container"));
    GtkWindow* main_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.mandelbrot"));


    /* GtkMenuBar *menubar = GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar")); */
    /* GtkMenuItem *fileMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_file")); */
    GtkMenuItem *settingsMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_settings"));
    GtkMenuItem *palettesMenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_palettes"));
    GtkMenuItem *btn_aether = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_aether"));
    GtkMenuItem *btn_fire = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_fire"));
    GtkMenuItem *btn_blue = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_blue"));
    GtkMenuItem *btn_midnight = GTK_MENU_ITEM(gtk_builder_get_object(builder, "item_pa_midnight"));
    GtkMenuItem *btn_file_new = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_new"));
    GtkMenuItem *btn_file_save = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_save"));
    GtkMenuItem *btn_file_save_hd = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_hd"));
    GtkMenuItem *btn_file_save_fhd = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_fhd"));
    GtkMenuItem *btn_file_save_sd = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_sd"));
    GtkMenuItem *btn_file_save_2hd = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_2hd"));
    GtkMenuItem *btn_file_quit = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_quit"));
    /* GtkMenu *submenu_file = GTK_MENU(gtk_builder_get_object(builder, "submenu_file")); */

	// Creating GTK Widgets for window : UNSAVED_CHANGES_POPUP
	GtkWindow* unsaved_changes_popup_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.unsaved_changes"));
	GtkButton* unsaved_changes_quit = GTK_BUTTON(gtk_builder_get_object(builder, "unsaved_changes_quit"));
	GtkButton* unsaved_changes_save = GTK_BUTTON(gtk_builder_get_object(builder, "unsaved_changes_save"));


    GtkWindow* about_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.about"));
    /* g_signal_connect(about_window, "destroy", G_CALLBACK(destroy_window), GTK_WIDGET(about_window)); */


    GtkMenuItem *btn_toolbar = GTK_MENU_ITEM(gtk_builder_get_object(builder, "btn_toolbar"));
    GtkBox* settings_panel = GTK_BOX(gtk_builder_get_object(builder, "settings_panel"));
    GtkScale* itr_scale = GTK_SCALE(gtk_builder_get_object(builder, "itr_scale"));
    GtkScale* theme_scale = GTK_SCALE(gtk_builder_get_object(builder, "theme_scale"));
    GtkScale* threads_scale = GTK_SCALE(gtk_builder_get_object(builder, "threads_scale"));
    GtkScale* steps_scale = GTK_SCALE(gtk_builder_get_object(builder, "steps_scale"));
    gtk_range_set_range(GTK_RANGE(threads_scale), 1,sysconf(_SC_NPROCESSORS_ONLN));
    gtk_range_set_value(GTK_RANGE(threads_scale), sysconf(_SC_NPROCESSORS_ONLN));
    GtkSwitch* transition_switch = GTK_SWITCH(gtk_builder_get_object(builder, "transition_switch"));
	GtkButton* btn_apply_settings = GTK_BUTTON(gtk_builder_get_object(builder, "btn_apply_settings"));


    os->area = area;
    os->settings_panel = settings_panel;
    os->settings->itr_scale = itr_scale;
    os->settings->theme_scale = theme_scale;

	// Connecting signal handlers
    g_signal_connect(area, "configure-event", G_CALLBACK(on_resize), os);
    gtk_widget_add_events(GTK_WIDGET(area), GDK_BUTTON_PRESS_MASK);
    g_signal_connect(area, "button_press_event", G_CALLBACK(coordinates), os);
    gtk_widget_add_events(GTK_WIDGET(area), GDK_SCROLL_MASK);
    g_signal_connect(area, "scroll-event", G_CALLBACK(on_scroll_event), os);
	g_signal_connect(area, "draw", G_CALLBACK(on_draw), os);
    g_signal_connect(G_OBJECT(itr_scale), "value-changed", G_CALLBACK(itr_scale_update), os);
    g_signal_connect(G_OBJECT(theme_scale), "value-changed", G_CALLBACK(theme_scale_update), os);
    g_signal_connect(G_OBJECT(threads_scale), "value-changed", G_CALLBACK(threads_scale_update), os);
    g_signal_connect(G_OBJECT(steps_scale), "value-changed", G_CALLBACK(steps_scale_update), os);
    g_signal_connect(G_OBJECT(transition_switch), "state-set", G_CALLBACK(set_transition), os);
    
    g_signal_connect(G_OBJECT(btn_file_new), "activate", G_CALLBACK(on_new), os);
    g_signal_connect(G_OBJECT(btn_file_save), "activate", G_CALLBACK(on_save), os);
    g_signal_connect(G_OBJECT(btn_file_save_hd), "activate", G_CALLBACK(on_save), os);
    g_signal_connect(G_OBJECT(btn_file_save_fhd), "activate", G_CALLBACK(on_save), os);
    g_signal_connect(G_OBJECT(btn_file_save_sd), "activate", G_CALLBACK(on_save), os);
    g_signal_connect(G_OBJECT(btn_file_save_2hd), "activate", G_CALLBACK(on_save), os);
    g_signal_connect(G_OBJECT(btn_toolbar), "activate", G_CALLBACK(switchPanel), os);
    g_signal_connect(G_OBJECT(btn_aether), "activate", G_CALLBACK(paletteAether), os);
    g_signal_connect(G_OBJECT(btn_fire), "activate", G_CALLBACK(paletteFire), os);
    g_signal_connect(G_OBJECT(btn_blue), "activate", G_CALLBACK(paletteBlue), os);
    g_signal_connect(G_OBJECT(btn_midnight), "activate", G_CALLBACK(paletteMidnight), os);
    g_signal_connect(G_OBJECT(btn_apply_settings), "clicked", G_CALLBACK(apply_settings), os);
    g_signal_connect(G_OBJECT(btn_file_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(GTK_WIDGET(main_window));
    gtk_widget_hide(GTK_WIDGET(settings_panel));

    gtk_main();

    return 0;
}
