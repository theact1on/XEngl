#ifndef CHECKS_H
#define CHECKS_H
#include <ctype.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
struct Item {
    char word[100];
    char translation[100];
};

void button_compare(GtkWidget* widget, GQueue* list);
void enter_compare(GtkWidget* widget, GQueue* list);
int compare_words(const char* answer, struct Item* right_word, int what_is);
int compare_structs(struct Item* first_word, struct Item* second_word);
_Bool check_valid_char(gint position, gchar character);
#endif