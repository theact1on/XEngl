#include "training.h"
#include "stats_win.h"
#include "vocabulary.h"

void training_win(GtkWidget* widget, gpointer data)
{
    srand(time(NULL));

    int num_of_words = count_words();
    int all_count_words = 0;
    int success_count_words = 0;
    int breakout = 0;

    task_function types_of_task[2] = {four_buttons_task, enter_translate_task};
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget *tr_box, *label_name_window, *task_box, *button_box, *btn_next, *btn_end;
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));

    gtk_container_remove(GTK_CONTAINER(window), main_box);
    tr_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), tr_box);

    label_name_window = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_name_window), "<span size=\"25000\">Тренировка</span>");
    gtk_box_pack_start(GTK_BOX(tr_box), label_name_window, FALSE, FALSE, 15);

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(tr_box), button_box, FALSE, FALSE, 0);
    gtk_widget_set_name(button_box, "button_box_train");

    task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(tr_box), task_box, TRUE, TRUE, 0);

    btn_end = gtk_button_new_with_label("Закончить");
    gtk_widget_set_margin_bottom(btn_end, 10);
    gtk_box_pack_start(GTK_BOX(button_box), btn_end, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_end), "clicked", gtk_main_quit, NULL);
    gtk_widget_set_sensitive(btn_end, FALSE);

    btn_next = gtk_button_new_with_label("Далее ->");
    gtk_widget_set_margin_bottom(btn_next, 10);
    gtk_box_pack_end(GTK_BOX(button_box), btn_next, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_next), "clicked", gtk_main_quit, NULL);
    gtk_widget_set_sensitive(btn_next, FALSE);

    while (!breakout) {
        int rand_type_task = rand() % 2;
        gtk_container_remove(GTK_CONTAINER(tr_box), task_box);
        all_count_words++;
        char bufer[130];
        sprintf(bufer, "<span size=\"35000\">%d</span>", all_count_words);
        gtk_label_set_markup(GTK_LABEL(label_name_window), bufer);
        task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX(tr_box), task_box, TRUE, TRUE, 30);
        gtk_box_reorder_child(GTK_BOX(tr_box), task_box, 1);
        types_of_task[rand_type_task](GTK_BOX(task_box), num_of_words, &success_count_words, btn_end, btn_next);
        gtk_widget_set_sensitive(btn_end, FALSE);
        gtk_widget_set_sensitive(btn_next, FALSE);
        gtk_widget_show_all(tr_box);
        gtk_main();
    }

    gtk_widget_destroy(tr_box);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_show_all(window);
}

void four_buttons_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next)
{
    GQueue* list;
    list = g_queue_new();
    g_queue_push_head(list, success_count_words);
    g_queue_push_head(list, btn_end);
    g_queue_push_head(list, btn_next);

    srand(time(NULL));
    GtkWidget *label_word, *btn_answer_box, *answer_buttons[4];
    struct Item its;
    struct Item fail;

    label_word = gtk_label_new("LOADING...");
    gtk_box_pack_start(task_box, label_word, FALSE, FALSE, 0);

    gtk_widget_set_name(label_word, "label_question");

    btn_answer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_box_pack_start(task_box, btn_answer_box, TRUE, TRUE, 0);

    const int NUM_BUTTONS = 7;

    int rand_word[7] = {-1};
    int type_its = rand() % 2 + 1;

    char bufer[130];
    rand_word[3] = rand() % N_WORDS;
    FILE* question_word = fopen("data/voc.dat", "rb");
    fseek(question_word, rand_word[3] * sizeof(struct Item), 0);
    fread(&its, sizeof(struct Item), 1, question_word);

    if (type_its == 1) {
        gtk_label_set_text(GTK_LABEL(label_word), its.word);
        sprintf(bufer, "<span size=\"35000\">%s</span>", its.word);
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
        answer_buttons[3] = gtk_button_new_with_label(its.translation);
    } else {
        gtk_label_set_text(GTK_LABEL(label_word), its.translation);
        sprintf(bufer, "<span size=\"35000\">%s</span>", its.translation);
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
        answer_buttons[3] = gtk_button_new_with_label(its.word);
    }

    g_queue_push_head(list, answer_buttons[3]);

    for (int i = 4; i < 7; i++) {
        do {
            rand_word[i] = rand() % N_WORDS;
        } while (rand_word[i] == rand_word[i - 1] || rand_word[i] == rand_word[i - 2] || rand_word[i] == rand_word[i - 3]);
        fseek(question_word, rand_word[i] * sizeof(struct Item), 0);
        fread(&fail, sizeof(struct Item), 1, question_word);

        if (type_its == 1) {
            answer_buttons[i - 4] = gtk_button_new_with_label(fail.translation);
        } else {
            answer_buttons[i - 4] = gtk_button_new_with_label(fail.word);
        }

        g_queue_push_head(list, answer_buttons[i - 4]);

        g_signal_connect(G_OBJECT(answer_buttons[i - 4]), "clicked", G_CALLBACK(failed_answer), list);
    }
    g_signal_connect(G_OBJECT(answer_buttons[3]), "clicked", G_CALLBACK(success_answer), list);
    shuffle_widgets(answer_buttons, 4);

    for (int i = 0; i < 4; i++)
        gtk_box_pack_start(GTK_BOX(btn_answer_box), answer_buttons[i], TRUE, TRUE, 0);
    fclose(question_word);
}

void enter_translate_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next)
{
    GtkWidget *label_word, *entry_answer_box, *entry_label, *button_success;
    srand(time(NULL));
    label_word = gtk_label_new("LOADING...");
    gtk_box_pack_start(task_box, label_word, FALSE, FALSE, 0);

    gtk_widget_set_name(label_word, "label_question");

    entry_answer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_box_pack_start(task_box, entry_answer_box, FALSE, FALSE, 0);
    button_success = gtk_button_new_with_label("Подтвердить");
    gtk_box_pack_end(GTK_BOX(entry_answer_box), button_success, FALSE, FALSE, 0);

    gtk_widget_set_margin_end(button_success, 120);
    gtk_widget_set_margin_start(button_success, 120);

    entry_label = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(entry_answer_box), entry_label, FALSE, FALSE, 0);
    gtk_widget_grab_focus(entry_label);
    gtk_widget_set_name(entry_label, "entry_label");
    gtk_widget_set_halign(entry_label, 0.5);
}

int count_words()
{
    FILE* file_count = fopen("data/voc.dat", "rb");
    struct Item* temp = g_malloc(sizeof(struct Item));
    int counter = 0;
    fread(temp, sizeof(struct Item), 1, file_count);
    while (!feof(file_count)) {
        counter++;
        fread(temp, sizeof(struct Item), 1, file_count);
    }
    gtk_main_iteration_do(gtk_events_pending());
    g_free(temp);
    fclose(file_count);
    return counter;
}

void shuffle_widgets(GtkWidget** arr, int N)
{
    srand(time(NULL));
    for (int i = N - 1; i >= 1; i--) {
        int j = rand() % (i + 1);

        GtkWidget* tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
    }
}

void failed_answer(GtkWidget* widget, GQueue* list)
{
    GtkWidget *btn_next, *btn_end, *answer_buttons[4];
    GList* child;
    child = g_queue_peek_head_link(list);
    for (int i = 0; i < 4; i++) {
        answer_buttons[i] = (GtkWidget*)(child->data);
        gtk_widget_set_sensitive(answer_buttons[i], FALSE);
        child = child->next;
    }
    btn_next = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(btn_next, TRUE);
    child = child->next;
    btn_end = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(btn_end, TRUE);
    gtk_widget_set_name(widget, "button_failed");
    gtk_widget_set_name(answer_buttons[3], "button_success");

    g_queue_free(list);
}

void success_answer(GtkWidget* widget, GQueue* list)
{
    GtkWidget *btn_next, *btn_end, *answer_buttons[4];
    GList* child;
    child = g_queue_peek_head_link(list);
    for (int i = 0; i < 4; i++) {
        answer_buttons[i] = (GtkWidget*)(child->data);
        gtk_widget_set_sensitive(answer_buttons[i], FALSE);
        child = child->next;
    }
    btn_next = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(btn_next, TRUE);
    child = child->next;
    btn_end = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(btn_end, TRUE);
    child = child->next;
    gtk_widget_set_name(widget, "button_success");
    int* success = (int*)(child->data);
    *success = *success + 1;
    g_queue_free(list);
}
