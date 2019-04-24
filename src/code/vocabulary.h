#ifndef VOCABULARY_H
#define VOCABULARY_H
#include <gtk/gtk.h>
// struct Item {
//     gchar* word;
//     gchar* translation;
// };
struct Item {
    char word[50];
    char translation[50];
};
void vocabulary_win(GtkWidget*, gpointer);
#endif