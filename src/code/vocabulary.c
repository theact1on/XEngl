#include "vocabulary.h"

void vocabulary_win(GtkWidget* widget, gpointer data)
{
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));
    gtk_container_remove(GTK_CONTAINER(window), main_box);

    GtkWidget* voc_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), voc_box);

    GtkWidget *label, *btns_box, *btn_add_rec, *btn_rem_rec, *btn_back, *btn_save, *treeview, *sw;

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span size=\"25000\" color=\"#c9281e\">Словарь</span>");
    gtk_box_pack_start(GTK_BOX(voc_box), label, FALSE, FALSE, 30);

    btns_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_end(GTK_BOX(voc_box), btns_box, FALSE, FALSE, 0);
    GtkWidget *btns_box1, *btns_box2;
    btns_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    btns_box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(btns_box), btns_box1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btns_box), btns_box2, FALSE, FALSE, 0);

    btn_add_rec = gtk_button_new_with_label("Добавить запись");
    gtk_widget_set_name(btn_add_rec, "btn_add");
    gtk_widget_set_margin_bottom(btn_add_rec, 10);
    gtk_box_pack_start(GTK_BOX(btns_box1), btn_add_rec, TRUE, TRUE, 15);

    btn_rem_rec = gtk_button_new_with_label("Удалить запись");
    gtk_widget_set_margin_bottom(btn_rem_rec, 10);
    gtk_box_pack_start(GTK_BOX(btns_box1), btn_rem_rec, TRUE, TRUE, 15);

    btn_back = gtk_button_new_with_label("Назад");
    gtk_widget_set_margin_bottom(btn_back, 10);
    gtk_box_pack_start(GTK_BOX(btns_box2), btn_back, TRUE, TRUE, 15);

    btn_save = gtk_button_new_with_label("Сохранить");
    gtk_widget_set_margin_bottom(btn_save, 10);
    gtk_box_pack_start(GTK_BOX(btns_box2), btn_save, TRUE, TRUE, 15);

    sw = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(voc_box), sw, TRUE, TRUE, 0);

    treeview = gtk_tree_view_new();
    /* Задание модели */
    GtkListStore* model;
    model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
    gtk_container_add(GTK_CONTAINER(sw), treeview);
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)), GTK_SELECTION_SINGLE);

    char* names_columns[2] = {"Слово", "Перевод"};
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* column;
    renderer = gtk_cell_renderer_text_new();
    for (int i = 0; i < 2; i++) {
        column = gtk_tree_view_column_new_with_attributes(names_columns[i], renderer, "text", i, NULL);
        gtk_tree_view_column_set_alignment(column, 0.5); // Выравнивание по центру
        gtk_tree_view_column_set_expand(column, TRUE);   // Равное разбиение между столбцами
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }

    /* Нажатия на кнопки */
    // g_signal_connect(G_OBJECT(btn_add_rec), "clicked", NULL, NULL);
    // g_signal_connect(G_OBJECT(btn_rem_rec), "clicked", NULL, NULL);
    g_signal_connect(G_OBJECT(btn_back), "clicked", gtk_main_quit, NULL);
    // g_signal_connect(G_OBJECT(btn_save), "clicked", NULL, NULL);

    /* Спиннер */
    GtkWidget* spinner = gtk_spinner_new();
    gtk_box_pack_end(GTK_BOX(voc_box), spinner, TRUE, TRUE, 30);

    gtk_widget_show(spinner);
    gtk_widget_show(voc_box);
    gtk_spinner_start(GTK_SPINNER(spinner));

    gtk_widget_destroy(spinner);
    gtk_widget_show_all(voc_box);
    gtk_main();

    gtk_widget_destroy(voc_box);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
}