#ifndef TRAINING_H
#define TRAINING_H
#include <ctype.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

typedef void (*task_function)(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next);
void training_win(GtkWidget* widget, gpointer data);
void four_buttons_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next);
void enter_translate_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next);
void shuffle_widgets(GtkWidget** arr, int N);
void failed_answer(GtkWidget* widget, GQueue* list);
int count_words();

#endif