#ifndef STATS_WIN_H
#define STATS_WIN_H
#include <gtk/gtk.h>
#include <stdlib.h>
struct stats_rec {
    char date[20];
    int suc_words;
    int all_words;
};
void stats_win(GtkWidget* widget, gpointer data);
void setup_table(GtkWidget* treeview);
#endif