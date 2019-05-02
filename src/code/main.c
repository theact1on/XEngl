#include "stats_win.h"
#include "training.h"
#include "vocabulary.h"
#include <gtk/gtk.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    GtkWidget *window, *main_box, *button_box, *main_buttons[4], *main_logo;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    main_buttons[0] = gtk_button_new_with_label("Начать");
    main_buttons[1] = gtk_button_new_with_label("Словарь");
    main_buttons[2] = gtk_button_new_with_label("Статистика");
    main_buttons[3] = gtk_button_new_with_label("Выход");
    main_logo = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_size("src/images/XEngl_logo.png", 500, 250, NULL));

    gtk_window_set_title(GTK_WINDOW(window), "XEngl");
    gtk_widget_set_size_request(GTK_WIDGET(window), 600, 700);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(exit), (gpointer)0);
    g_signal_connect(G_OBJECT(main_buttons[0]), "clicked", G_CALLBACK(training_win), window);
    g_signal_connect(G_OBJECT(main_buttons[1]), "clicked", G_CALLBACK(vocabulary_win), window);
    g_signal_connect(G_OBJECT(main_buttons[2]), "clicked", G_CALLBACK(stats_win), window);
    g_signal_connect_swapped(G_OBJECT(main_buttons[3]), "clicked", G_CALLBACK(exit), (gpointer)0);

    gtk_box_pack_start(GTK_BOX(main_box), main_logo, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(main_box), button_box, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), main_buttons[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), main_buttons[1], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), main_buttons[2], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), main_buttons[3], TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}