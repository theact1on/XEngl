#include "checks.h"

_Bool check_valid_char(gint position, gchar character)
{
    gchar ignored_characters[] = "0123456789!@#$%^&*()_+'`;:?.<>,[]{}()=/\"\\|";
    for (int j = 0; j < strlen(ignored_characters); j++) {
        if (position == 0)
            if (character == ' ') {
                return TRUE;
                break;
            }
        if (character == ignored_characters[j]) {
            return TRUE;
        }
    }
    return FALSE;
}

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
    GtkWidget *btn_next, *btn_end, *btn_success, *entry_label, *right_word;
    struct Item* its;
    GList* child;
    child = g_queue_peek_head_link(list);

    int* what_is = (int*)(child->data);

    child = child->next;
    its = (struct Item*)(child->data);
    child = child->next;

    entry_label = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(entry_label, FALSE);
    child = child->next;
    btn_success = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(btn_success, FALSE);
    child = child->next;
    right_word = (GtkWidget*)(child->data);
    child = child->next;
    btn_next = (GtkWidget*)(child->data);
    child = child->next;
    btn_end = (GtkWidget*)(child->data);
    child = child->next;

    if (compare_words(gtk_entry_get_text(GTK_ENTRY(entry_label)), its, *what_is)) {
        int* success = (int*)(child->data);
        *success = *success + 1;
        gtk_widget_set_name(btn_success, "button_success");
        gtk_widget_set_name(entry_label, "entry_success");
    } else {
        char bufer[130];
        if (*what_is == 1) {
            sprintf(bufer, "<span size=\"30000\">%s</span>", g_utf8_strup(its->translation, -1));
        } else {
            sprintf(bufer, "<span size=\"30000\">%s</span>", g_utf8_strup(its->word, -1));
        }
        gtk_label_set_markup(GTK_LABEL(right_word), bufer);
        gtk_widget_set_name(btn_success, "button_failed");
        gtk_widget_set_name(entry_label, "entry_failed");
    }
    gtk_widget_set_sensitive(btn_next, TRUE);
    gtk_widget_set_sensitive(btn_end, TRUE);
    g_queue_free(list);
}

int compare_words(const char* answer, struct Item* right_word, int what_is)
{
    if (what_is == 1) {
        if (g_utf8_collate(g_utf8_strdown(answer, -1), g_utf8_strdown(right_word->translation, -1)) == 0) {
            return 1;
        }
    } else {
        if (g_utf8_collate(g_utf8_strdown(answer, -1), g_utf8_strdown(right_word->word, -1)) == 0) {
            return 1;
        }
    }
    return 0;
}

int compare_structs(struct Item* first_word, struct Item* second_word)
{
    if (g_utf8_collate(g_utf8_strdown(first_word->word, -1), g_utf8_strdown(second_word->word, -1)) == 0
        || g_utf8_collate(g_utf8_strdown(first_word->translation, -1), g_utf8_strdown(second_word->translation, -1)) == 0) {
        return 1;
    }
    return 0;
}