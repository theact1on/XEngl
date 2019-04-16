#include <gtk/gtk.h>

int main(int argc, char* argv[])
{
    GtkWidget* window;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "XEngl");
    gtk_widget_set_size_request(GTK_WIDGET(window), 600, 700);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(exit), (gpointer)0);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}