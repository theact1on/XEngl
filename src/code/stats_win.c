#include "stats_win.h"

void stats_win(GtkWidget* widget, gpointer data)
{
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget* stats_box;
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));

    gtk_container_remove(GTK_CONTAINER(window), main_box);
    stats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), stats_box);

    gtk_main();

    gtk_widget_destroy(stats_box);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
    return;
}