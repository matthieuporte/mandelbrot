#include <gtk/gtk.h>
#include <time.h>
#include "struct.h"
#include "draw.h"

gboolean on_save(GtkMenuItem *item, OverallState* os){
    GError *error = NULL;

    int w;
    int h;
    GdkPixbuf* hdPixbuf;

    if (strcmp(gtk_menu_item_get_label(item),"Save") == 0){
        w = os->state->w;
        h = os->state->h;
    } else {
        if (strcmp(gtk_menu_item_get_label(item),"1920*1080") == 0){
            w = 1920;
            h = 1080;
        } else if (strcmp(gtk_menu_item_get_label(item),"960*540") == 0){
            w = 960;
            h = 540;
        } else if (strcmp(gtk_menu_item_get_label(item),"2560*1440") == 0){
            w = 2560;
            h = 1440;
        } else if (strcmp(gtk_menu_item_get_label(item),"3840*2160") == 0){
            w = 3840;
            h = 2160;
        }
        hdPixbuf = gdk_pixbuf_scale_simple(gdk_pixbuf_copy(os->state->colorBuf),
            w, h, GDK_INTERP_BILINEAR);

        custom_render(hdPixbuf,w,h,os);
    }

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Extract components
    int year = timeinfo->tm_year + 1900; // tm_year is the number of years since 1900
    int month = timeinfo->tm_mon + 1;    // tm_mon is 0-based (0 is January)
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;
    int second = timeinfo->tm_sec;

    /* gchar *expanded_filename = g_build_filename(g_get_home_dir(), ".mandelbrotSaves/mand.png", NULL); */
    gchar *test_dir1 = g_build_filename(g_get_home_dir(), "Pictures", NULL);
    gchar *test_dir2 = g_build_filename(g_get_home_dir(), "pictures", NULL);
    gchar *test_dir3 = g_build_filename(g_get_home_dir(), "Images", NULL);
    gchar *test_dir4 = g_build_filename(g_get_home_dir(), "Mandelbrot_Saves", NULL);

    gchar *dir1;

    if (g_file_test(test_dir1, G_FILE_TEST_IS_DIR)) {
        dir1 = test_dir1;
    } else if (g_file_test(test_dir2, G_FILE_TEST_IS_DIR)){
        dir1 = test_dir2;
    } else if (g_file_test(test_dir3, G_FILE_TEST_IS_DIR)){
        dir1 = test_dir3;
    } else {
        dir1 = "";
    }

    gchar* true_dir;
    asprintf(&true_dir, "%s/Mandelbrot_Saves",dir1);

    if (!g_file_test(true_dir, G_FILE_TEST_IS_DIR)){
        g_mkdir_with_parents(true_dir, 0755);
    }

    char *filename;
    int res = asprintf(&filename, "%s/mandelbrot-%d-%02d-%02d_%02d-%02d-%02d.png", true_dir, year, month,day,hour,minute,second);


    if (strcmp(gtk_menu_item_get_label(item),"Save") != 0) {
        if (!gdk_pixbuf_save(hdPixbuf, filename, "png", &error, NULL)) {
            g_printerr("Error saving Pixbuf to file: %s\n", error->message);
            g_error_free(error);
        }
        g_object_unref(hdPixbuf);
    } else {

        if (!gdk_pixbuf_save(os->state->colorBuf, filename, "png", &error, NULL)) {
            g_printerr("Error saving Pixbuf to file: %s\n", error->message);
            g_error_free(error);
        }
    }


    free(filename); 
    return FALSE;
}
