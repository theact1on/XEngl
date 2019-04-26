#include "vocabulary.h"

void read_from_bfile(GtkListStore* model)
{
    /* Чтение из файла */
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));
    GtkTreeIter iter;
    FILE* file = fopen("data/voc.dat", "rb");

    if (file == NULL) {
        file = fopen("data/voc.dat", "wb");
        fclose(file);
        file = fopen("data/voc.dat", "rb");
    }
    fread(item, sizeof(struct Item), 1, file);
    while (file != NULL && !feof(file)) {
        gtk_list_store_append(model, &iter);
        gtk_list_store_set(model, &iter, 0, item->word, 1, item->translation, -1);
        gtk_main_iteration_do(gtk_events_pending());
        fread(item, sizeof(struct Item), 1, file);
    }
    fclose(file);
    file = NULL;
}

void write_to_bfile(GtkWidget* button, gpointer data)
{
    struct Item item;

    GtkTreeView* treeview = (GtkTreeView*)data;
    GtkTreeModel* model = gtk_tree_view_get_model(treeview);

    GtkTreeIter iter;
    gtk_tree_model_get_iter_first(model, &iter);

    while (gtk_tree_view_column_get_sort_order(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0)) != 1) {
        gtk_tree_view_column_clicked(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0));
    }
    gtk_tree_view_column_clicked(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0));
    // g_print("%d \n", gtk_tree_view_column_get_sort_order(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0)));

    gboolean valid;
    valid = gtk_tree_model_get_iter_first(model, &iter);
    FILE* file;
    gchar* word;
    gchar* translation;
    file = fopen("data/temp.dat", "wb"); // открытие бинарного файла для записи
    while (valid) {
        gtk_tree_model_get(model, &iter, 0, &(word), 1, &(translation), -1);
        strcpy(item.word, word);
        strcpy(item.translation, translation);
        // g_print("%s\t%s\n", word, translation);
        fwrite(&item, sizeof(item), 1, file); // запись в файл структуры
        valid = gtk_tree_model_iter_next(model, &iter);
    }
    fclose(file);
    remove("data/voc.dat");
    rename("data/temp.dat", "data/voc.dat");
    gtk_main_quit();
}

void add_item(GtkWidget* button, gpointer data)
{
    GtkTreeView* treeview = (GtkTreeView*)data;
    GtkTreeIter current, iter;
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));

    GtkTreeModel* model = gtk_tree_view_get_model(treeview);
    GtkTreeViewColumn* column;

    /* Вставка новой строки после текущей */
    GtkTreePath* path;
    gtk_tree_view_get_cursor(treeview, &path, NULL);
    if (path) {
        gtk_tree_model_get_iter(model, &current, path);
        gtk_tree_path_free(path);
        gtk_list_store_insert_after(GTK_LIST_STORE(model), &iter, &current);
    } else {
        gtk_list_store_insert(GTK_LIST_STORE(model), &iter, -1);
    }

    /* Установка данных для новой строки*/
    strcpy(item->word, "<Слово>");
    strcpy(item->translation, "<Перевод>");

    gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, item->word, 1, item->translation, -1);

    /* Перемещение выделения на созданную строку */
    path = gtk_tree_model_get_path(model, &iter);
    column = gtk_tree_view_get_column(treeview, 0);
    gtk_tree_view_set_cursor(treeview, path, column, FALSE);

    gtk_tree_path_free(path);
}

void remove_item(GtkWidget* widget, gpointer data)
{
    GtkTreeView* treeview = (GtkTreeView*)data;
    GtkTreeIter iter;

    GtkTreeModel* model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);

    if (gtk_tree_selection_get_selected(selection, NULL, &iter))
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
}

void cell_edited(GtkCellRendererText* cell, const gchar* path_string, const gchar* new_text, gpointer data)
{
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));

    GtkTreeModel* model = (GtkTreeModel*)data;

    GtkTreePath* path = gtk_tree_path_new_from_string(path_string);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);
    gtk_tree_path_free(path);

    gint column = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(cell), "column"));
    // g_print("%d     %p     %s      %p\n", column, cell, path_string, data);
    switch (column) {
    case 0: {
        strcpy(item->word, new_text);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, item->word, -1);
    } break;

    case 1: {
        strcpy(item->translation, new_text);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, item->translation, -1);
    } break;
    }
}
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
    /* Задание модели таблицы */
    GtkListStore* model;
    model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
    gtk_container_add(GTK_CONTAINER(sw), treeview);
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)), GTK_SELECTION_SINGLE);

    char* names_columns[2] = {"Слово", "Перевод"};
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* column;

    for (int i = 0; i < 2; i++) {
        renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "editable", TRUE, NULL); // Редактирование строки
        g_signal_connect(renderer, "edited", G_CALLBACK(cell_edited), GTK_TREE_MODEL(model));
        g_object_set_data(G_OBJECT(renderer), "column", GINT_TO_POINTER(i));
        column = gtk_tree_view_column_new_with_attributes(names_columns[i], renderer, "text", i, NULL);
        gtk_tree_view_column_set_sort_column_id(column, i);
        gtk_tree_view_column_set_alignment(column, 0.5); // Выравнивание по центру
        gtk_tree_view_column_set_expand(column, TRUE);   // Равное разбиение между столбцами
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
    gtk_tree_view_column_clicked(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0));

    /* Нажатия на кнопки */
    g_signal_connect(G_OBJECT(btn_add_rec), "clicked", G_CALLBACK(add_item), treeview);
    g_signal_connect(G_OBJECT(btn_rem_rec), "clicked", G_CALLBACK(remove_item), treeview);
    g_signal_connect(G_OBJECT(btn_back), "clicked", gtk_main_quit, NULL);
    g_signal_connect(G_OBJECT(btn_save), "clicked", G_CALLBACK(write_to_bfile), treeview);

    /* Спиннер */
    GtkWidget* spinner = gtk_spinner_new();
    gtk_box_pack_end(GTK_BOX(voc_box), spinner, TRUE, TRUE, 30);

    gtk_widget_show(spinner);
    gtk_widget_show(voc_box);
    gtk_spinner_start(GTK_SPINNER(spinner));

    read_from_bfile(model);

    gtk_widget_destroy(spinner);
    gtk_widget_show_all(voc_box);
    gtk_main();

    gtk_widget_destroy(voc_box);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
}