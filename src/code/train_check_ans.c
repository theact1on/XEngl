#include "train_check_ans.h"

void button_compare(GtkWidget* widget, GQueue* list)
{
    struct Item* right_word;
    GtkWidget *btn_next, *btn_end, *answer_buttons[4];
    GList* child;
    int* what_is;
    child = g_queue_peek_head_link(list);
    for (int i = 0; i < 4; i++) {
        answer_buttons[i] = (GtkWidget*)(child->data);
        gtk_widget_set_sensitive(answer_buttons[i], FALSE);
        child = child->next;
    }
    right_word = (struct Item*)(child->data);
    child = child->next;
    what_is = (int*)(child->data);
    child = child->next;
    btn_next = (GtkWidget*)(child->data);
    child = child->next;
    btn_end = (GtkWidget*)(child->data);
    child = child->next;
    int* success = (int*)(child->data);
    g_queue_free(list);

    if (compare_words(gtk_button_get_label(GTK_BUTTON(widget)), right_word, *what_is)) {
        gtk_widget_set_name(widget, "button_success");
        *success = *success + 1;
    } else {
        gtk_widget_set_name(answer_buttons[3], "button_success");
        gtk_widget_set_name(widget, "button_failed");
    }

    gtk_widget_set_sensitive(btn_next, TRUE);
    gtk_widget_set_sensitive(btn_end, TRUE);
}

void enter_compare(GtkWidget* widget, GQueue* list)
{
}

int compare_words(const char* answer, struct Item* right_word, int what_is)
{
}

int compare_structs(struct Item* first_word, struct Item* second_word)
{
}