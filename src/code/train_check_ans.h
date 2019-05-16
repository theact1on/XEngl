#ifndef TRAIN_CHECK_ANS_H
#define TRAIN_CHECK_ANS_H
#include "vocabulary.h"
#include <ctype.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
void button_compare(GtkWidget* widget, GQueue* list);
void enter_compare(GtkWidget* widget, GQueue* list);
int compare_words(const char* answer, struct Item* right_word, int what_is);
int compare_structs(struct Item* first_word, struct Item* second_word);
#endif