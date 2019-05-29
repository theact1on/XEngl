#ifndef VOCABULARY_H
#define VOCABULARY_H
#include "checks.h"
#include "default_words.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

gboolean key_press_event_voc(GtkWidget* view, GdkEventKey* event);
void vocabulary_win(GtkWidget*, gpointer);
void read_from_bfile(GtkListStore* model);
gint sortsave(GtkTreeModel* model, GtkTreeIter* a, GtkTreeIter* b, gpointer user_data);
void write_to_bfile(GtkWidget* button, gpointer data);
void add_item(GtkWidget* button, gpointer data);
void remove_item(GtkWidget* widget, gpointer data);
void insert_text(GtkEntry* entry, const gchar* text, gint len, gint* position, gpointer data);
void cell_edit(GtkCellRenderer* renderer, GtkCellEditable* editable, gchar* path, gpointer user_data);
void cell_edited(GtkCellRendererText* cell, const gchar* path_string, const gchar* new_text, gpointer data);
#endif