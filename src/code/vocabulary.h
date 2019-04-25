#ifndef VOCABULARY_H
#define VOCABULARY_H
#include <gtk/gtk.h>
#include <stdlib.h>

struct Item {
    char word[100];
    char translation[100];
};

void vocabulary_win(GtkWidget*, gpointer);
#endif