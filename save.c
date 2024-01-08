#include <gtk/gtk.h>
#include <time.h>
#include "struct.h"
#include "draw.h"

gboolean on_save(GtkMenuItem *item, OverallState* os){
    GError *error = NULL;

    /* int w = os->state->w; */
    /* int h = os->state->h; */

    GdkPixbuf* hdPixbuf = gdk_pixbuf_scale_simple(gdk_pixbuf_copy(os->state->colorBuf),
            1920, 1080, GDK_INTERP_BILINEAR);

    hd_render(hdPixbuf,os);

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
    int res = asprintf(&filename, "%s/mandelbrot-%d-%1d-%1d_%1d-%1d-%1d.png", true_dir, year, month,day,hour,minute,second);

    // Save the Pixbuf to the specified file
    if (!gdk_pixbuf_save(hdPixbuf, filename, "png", &error, NULL)) {
        // Handle the error, e.g., print an error message
        g_printerr("Error saving Pixbuf to file: %s\n", error->message);
        g_error_free(error);
    }
    g_object_unref(hdPixbuf);

    free(filename); 
    return FALSE;
}
