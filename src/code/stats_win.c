#include "stats_win.h"

void stats_win(GtkWidget* widget, gpointer data)
{
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget *stats_box, *btn_box, *btn_back, *btn_del_stats, *treeview, *scrolled_win, *label, *spinner;
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));
    GtkListStore* store;
    GtkTreeIter iter;

    gtk_container_remove(GTK_CONTAINER(window), main_box);
    stats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), stats_box);

    label = gtk_label_new(NULL);
    gtk_widget_set_name(label, "header");
    gtk_label_set_markup(GTK_LABEL(label), "Статистика");
    gtk_box_pack_start(GTK_BOX(stats_box), label, FALSE, FALSE, 30);

    btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_end(GTK_BOX(stats_box), btn_box, FALSE, FALSE, 0);

    btn_back = gtk_button_new_with_label("Назад");
    gtk_widget_set_margin_bottom(btn_back, 10);
    gtk_box_pack_start(GTK_BOX(btn_box), btn_back, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_back), "clicked", gtk_main_quit, NULL);

    btn_del_stats = gtk_button_new_with_label("Обнулить статистику");
    gtk_widget_set_margin_bottom(btn_del_stats, 10);
    gtk_box_pack_end(GTK_BOX(btn_box), btn_del_stats, TRUE, TRUE, 15);

    treeview = gtk_tree_view_new();
    setup_table(treeview);

    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_set_border_width(GTK_CONTAINER(scrolled_win), 15);
    gtk_box_pack_start(GTK_BOX(stats_box), scrolled_win, TRUE, TRUE, 0);

    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled_win), treeview);

    g_signal_connect(G_OBJECT(btn_del_stats), "clicked", G_CALLBACK(del_stats), (gpointer)store);

    spinner = gtk_spinner_new();
    gtk_box_pack_end(GTK_BOX(stats_box), spinner, TRUE, TRUE, 30);

    gtk_widget_show(spinner);
    gtk_widget_show(stats_box);
    gtk_spinner_start(GTK_SPINNER(spinner));

    FILE* stats = fopen("data/stats.dat", "rb");
    if (stats == NULL) {
        stats = fopen("data/stats.dat", "wb");
        fclose(stats);
        stats = fopen("data/stats.dat", "rb");
    }

    struct stats_rec* user_stats;
    user_stats = (struct stats_rec*)malloc(sizeof(struct stats_rec));

    fread(user_stats, sizeof(struct stats_rec), 1, stats);
    if (feof(stats)) {
        gtk_widget_set_sensitive(btn_del_stats, FALSE);
    }
    char bufer[7];
    while (stats != NULL && !feof(stats)) {
        gtk_list_store_prepend(store, &iter);
        sprintf(bufer, "%.2f", (float)user_stats->suc_words / user_stats->all_words * 100);
        gtk_list_store_set(store, &iter, 0, user_stats->date, 1, user_stats->suc_words, 2, user_stats->all_words, 3, bufer, -1);
        gtk_main_iteration_do(gtk_events_pending());
        fread(user_stats, sizeof(struct stats_rec), 1, stats);
    }
    fclose(stats);
    stats = NULL;

    g_object_unref(store);
    gtk_widget_destroy(spinner);
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

void del_stats(GtkWidget* widget, gpointer data)
{
    GtkListStore* store = (GtkListStore*)data;
    gtk_list_store_clear(store);
    remove("data/stats.dat");
    gtk_widget_set_sensitive(widget, FALSE);
    return;
}