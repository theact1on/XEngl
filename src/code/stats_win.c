#include "stats_win.h"

void stats_win(GtkWidget* widget, gpointer data)
{
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget *stats_box, *btn_box, *btn_back, *btn_del_stats;
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));

    gtk_container_remove(GTK_CONTAINER(window), main_box);
    stats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), stats_box);

    btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(stats_box), btn_box);

    btn_back = gtk_button_new_with_label("Назад");
    gtk_widget_set_margin_bottom(btn_back, 10);
    gtk_box_pack_start(GTK_BOX(btn_box), btn_back, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_back), "clicked", gtk_main_quit, NULL);

    btn_del_stats = gtk_button_new_with_label("Обнулить статистику");
    gtk_widget_set_margin_bottom(btn_del_stats, 10);
    gtk_box_pack_end(GTK_BOX(btn_box), btn_del_stats, TRUE, TRUE, 15);

    gtk_widget_show_all(stats_box);

    gtk_main();

    gtk_widget_destroy(stats_box);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
    return;
}

void setup_table(GtkWidget* treeview)
{
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* column;

    char* names_columns[4] = {"Дата&Время", "Отгаданных слов", "Всего слов", "% успешных ответов"};

    for (int i = 0; i < 4; i++) {
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(names_columns[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
    return;
}