#ifndef TRAINING_H
#define TRAINING_H
#include <ctype.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

void training_win(GtkWidget* widget, gpointer data);
void four_buttons_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next);
void enter_translate_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next);

#endif