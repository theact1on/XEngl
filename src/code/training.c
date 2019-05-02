#include "training.h"
#include "stats_win.h"
#include "vocabulary.h"

void training_win(GtkWidget* widget, gpointer data)
{
    task_function types_of_task[2] = {four_buttons_task, enter_translate_task};
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget *tr_box, *label_name_window, *task_box, *button_box, *btn_next, *btn_end;
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));

    gtk_container_remove(GTK_CONTAINER(window), main_box);
    tr_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), tr_box);

    label_name_window = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_name_window), "<span size=\"25000\">Тренировка</span>");
    gtk_box_pack_start(GTK_BOX(tr_box), label_name_window, FALSE, FALSE, 15);

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(tr_box), button_box, FALSE, FALSE, 0);
    gtk_widget_set_name(button_box, "button_box_train");

    task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(tr_box), task_box, TRUE, TRUE, 0);

    btn_end = gtk_button_new_with_label("Закончить");
    gtk_widget_set_margin_bottom(btn_end, 10);
    gtk_box_pack_start(GTK_BOX(button_box), btn_end, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_end), "clicked", gtk_main_quit, NULL);
    gtk_widget_set_sensitive(btn_end, FALSE);

    btn_next = gtk_button_new_with_label("Далее ->");
    gtk_widget_set_margin_bottom(btn_next, 10);
    gtk_box_pack_end(GTK_BOX(button_box), btn_next, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_next), "clicked", gtk_main_quit, NULL);
    gtk_widget_set_sensitive(btn_next, FALSE);

    gtk_widget_show_all(tr_box);
    gtk_main();

    gtk_widget_destroy(tr_box);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
}

void four_buttons_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next)
{
}

void enter_translate_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next)
{
}