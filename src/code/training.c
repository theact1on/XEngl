#include "training.h"
#include "default_words.h"
#include "stats_win.h"
#include "train_check_ans.h"
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

    GQueue* list;
    list = g_queue_new();
    g_queue_push_head(list, &all_count_words);
    g_queue_push_head(list, &success_count_words);
    g_queue_push_head(list, &breakout);
    g_queue_push_head(list, tr_box);

    // g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(results_win), list);

    btn_end = gtk_button_new_with_label("Закончить");
    gtk_widget_set_margin_bottom(btn_end, 10);
    gtk_box_pack_start(GTK_BOX(button_box), btn_end, TRUE, TRUE, 15);
    g_signal_connect(G_OBJECT(btn_end), "clicked", G_CALLBACK(results_win), list);
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
    struct Item* its = (struct Item*)malloc(sizeof(struct Item));
    struct Item fail;

    label_word = gtk_label_new("LOADING...");
    gtk_box_pack_start(task_box, label_word, FALSE, FALSE, 0);

    gtk_widget_set_name(label_word, "label_question");

    btn_answer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_box_pack_start(task_box, btn_answer_box, TRUE, TRUE, 0);

    const int NUM_BUTTONS = 7;

    int rand_word[7] = {-1};
    struct Item rand_word_item[7] = {-1};
    int* type_its = (int*)malloc(sizeof(int));
    *type_its = rand() % 2 + 1;

    g_queue_push_head(list, type_its);

    char bufer[130];
    rand_word[3] = rand() % N_WORDS + NUM_DEF_WORDS;
    FILE* question_word = fopen("data/voc.dat", "rb");
    fseek(question_word, rand_word[3] * sizeof(struct Item), 0);
    fread(its, sizeof(struct Item), 1, question_word);
    rand_word_item[3] = *its;

    g_queue_push_head(list, its);

    if (*type_its == 1) {
        gtk_label_set_text(GTK_LABEL(label_word), its->word);
        sprintf(bufer, "<span size=\"35000\">%s</span>", its->word);
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
        answer_buttons[3] = gtk_button_new_with_label(its->translation);
    } else {
        gtk_label_set_text(GTK_LABEL(label_word), its->translation);
        sprintf(bufer, "<span size=\"35000\">%s</span>", its->translation);
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
        answer_buttons[3] = gtk_button_new_with_label(its->word);
    }

    g_queue_push_head(list, answer_buttons[3]);

    for (int i = 4; i < 7; i++) {
        do {
            rand_word[i] = rand() % (N_WORDS + NUM_DEF_WORDS);
            fseek(question_word, rand_word[i] * sizeof(struct Item), 0);
            fread(&fail, sizeof(struct Item), 1, question_word);
            rand_word_item[i] = fail;
        } while (compare_structs(&rand_word_item[i], &rand_word_item[i - 1]) || compare_structs(&rand_word_item[i], &rand_word_item[i - 2])
                 || compare_structs(&rand_word_item[i], &rand_word_item[i - 3]));

        if (*type_its == 1) {
            answer_buttons[i - 4] = gtk_button_new_with_label(fail.translation);
        } else {
            answer_buttons[i - 4] = gtk_button_new_with_label(fail.word);
        }

        g_queue_push_head(list, answer_buttons[i - 4]);

        g_signal_connect(G_OBJECT(answer_buttons[i - 4]), "clicked", G_CALLBACK(button_compare), list);
    }
    g_signal_connect(G_OBJECT(answer_buttons[3]), "clicked", G_CALLBACK(button_compare), list);
    shuffle_widgets(answer_buttons, 4);

    for (int i = 0; i < 4; i++)
        gtk_box_pack_start(GTK_BOX(btn_answer_box), answer_buttons[i], TRUE, TRUE, 0);
    fclose(question_word);
}

void enter_translate_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next)
{
    GQueue* list;
    list = g_queue_new();
    g_queue_push_head(list, success_count_words);
    g_queue_push_head(list, btn_end);
    g_queue_push_head(list, btn_next);

    GtkWidget *label_word, *entry_answer_box, *entry_label, *button_success, *right_word;
    srand(time(NULL));
    label_word = gtk_label_new("LOADING...");
    gtk_box_pack_start(task_box, label_word, FALSE, FALSE, 0);

    gtk_widget_set_name(label_word, "label_question");

    entry_answer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_box_pack_start(task_box, entry_answer_box, FALSE, FALSE, 0);

    gtk_widget_set_name(entry_answer_box, "entry_answer_box");

    right_word = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(entry_answer_box), right_word, FALSE, FALSE, 0);

    gtk_widget_set_name(right_word, "right_word_hide");

    g_queue_push_head(list, right_word);
    button_success = gtk_button_new_with_label("Подтвердить");
    gtk_box_pack_end(GTK_BOX(entry_answer_box), button_success, FALSE, FALSE, 0);

    g_queue_push_head(list, button_success);

    entry_label = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(entry_answer_box), entry_label, FALSE, FALSE, 0);
    gtk_widget_grab_focus(entry_label);
    gtk_widget_set_halign(entry_label, 0.5);

    g_queue_push_head(list, entry_label);

    struct Item* its = (struct Item*)g_malloc(sizeof(struct Item));

    int rand_word = rand() % N_WORDS;
    int type_its = rand() % 2 + 1;
    FILE* question_word = fopen("data/voc.dat", "rb");
    fseek(question_word, rand_word * sizeof(struct Item), 0);
    fread(its, sizeof(struct Item), 1, question_word);

    if (type_its == 2) {
        char temp[120];
        strcpy(temp, its->translation);
        strcpy(its->translation, its->word);
        strcpy(its->word, temp);
    }

    gtk_label_set_text(GTK_LABEL(label_word), its->translation);
    char bufer[130];
    sprintf(bufer, "<span size=\"35000\">%s</span>", its->translation);
    gtk_label_set_markup(GTK_LABEL(label_word), bufer);
    fclose(question_word);

    g_queue_push_head(list, its);
    g_signal_connect(G_OBJECT(entry_label), "activate", G_CALLBACK(check_answer_entry), list);
    g_signal_connect(G_OBJECT(button_success), "clicked", G_CALLBACK(check_answer_entry), list);
}

int count_words()
{
    long i;
    int counter = 0;
    FILE* file_count = fopen("data/voc.dat", "rb");
    if (file_count == NULL) {
        file_count = fopen("data/voc.dat", "wb");
        file_count = def_words(file_count);
        fclose(file_count);
        file_count = fopen("data/voc.dat", "rb");
    }
    fseek(file_count, 0, 2);
    i = ftell(file_count);
    counter = i / sizeof(struct Item);
    fclose(file_count);
    return counter - NUM_DEF_WORDS;
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

void check_answer_entry(GtkWidget* widget, GQueue* list)
{
    GtkWidget *btn_next, *btn_end, *btn_success, *entry_label, *right_word;
    struct Item* its;
    GList* child;
    child = g_queue_peek_head_link(list);

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
    gtk_widget_set_sensitive(btn_next, TRUE);
    child = child->next;
    btn_end = (GtkWidget*)(child->data);
    gtk_widget_set_sensitive(btn_end, TRUE);
    child = child->next;
    char* entry = (char*)gtk_entry_get_text(GTK_ENTRY(entry_label));
    char* its_tr = its->word;

    if (g_utf8_collate(g_utf8_strdown(its_tr, -1), g_utf8_strdown(entry, -1)) == 0) {
        int* success = (int*)(child->data);
        *success = *success + 1;
        gtk_widget_set_name(btn_success, "button_success");
        gtk_widget_set_name(entry_label, "entry_success");
    } else {
        char bufer[130];
        sprintf(bufer, "<span size=\"30000\">%s</span>", its_tr);
        gtk_widget_set_name(right_word, "right_word_unhide");
        gtk_label_set_markup(GTK_LABEL(right_word), bufer);
        gtk_widget_set_name(btn_success, "button_failed");
        gtk_widget_set_name(entry_label, "entry_failed");
    }
    gtk_widget_set_sensitive(btn_success, FALSE);
    g_queue_free(list);
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

void results_win(GtkWidget* widget, GQueue* list)
{
    struct tm* u;
    char* f;
    GtkWidget *label_name_window, *btn_to_menu, *label_all_words, *label_success_words, *label_pecent;
    GList *children, *iter;
    GList* child = g_queue_peek_head_link(list);
    GtkWidget* tr_box = (GtkWidget*)(child->data);
    child = child->next;
    int* breakout = (int*)(child->data);
    child = child->next;
    int success_count_words = *((int*)(child->data));
    child = child->next;
    int all_count_words = *((int*)(child->data));
    *breakout = 1;
    float pecent_words;

    struct stats_rec* stats_item = (struct stats_rec*)malloc(sizeof(struct stats_rec));

    const time_t timer = time(NULL);
    u = localtime(&timer);
    f = settime(u);

    strcpy(stats_item->date, f);
    stats_item->suc_words = success_count_words;
    stats_item->all_words = all_count_words;

    FILE* setup_stats = fopen("data/stats.dat", "ab");

    if (setup_stats == NULL) {
        setup_stats = fopen("data/stats.dat", "wb");
        fclose(setup_stats);
        setup_stats = fopen("data/stats.dat", "ab");
    }

    fwrite(stats_item, sizeof(struct stats_rec), 1, setup_stats);
    fclose(setup_stats);

    if (success_count_words == 0) {
        pecent_words = 0;
    } else {
        pecent_words = (float)success_count_words / all_count_words * 100;
    }
    children = gtk_container_get_children(GTK_CONTAINER(tr_box));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    gchar* bufer = (gchar*)g_malloc(200 * sizeof(gchar));

    label_name_window = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_name_window), "<span size=\"25000\">Результаты тренировки</span>\n");
    gtk_box_pack_start(GTK_BOX(tr_box), label_name_window, FALSE, FALSE, 30);
    gtk_widget_set_name(label_name_window, "label_name_window_results");

    sprintf(bufer, "<span size=\"15000\">Общее кол-во слов</span>\n<span size=\"35000\">%d</span>", all_count_words);
    label_all_words = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_all_words), bufer);
    gtk_label_set_justify(GTK_LABEL(label_all_words), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(tr_box), label_all_words, TRUE, TRUE, 30);

    sprintf(bufer, "<span size=\"15000\">Кол-во правильных ответов</span>\n<span size=\"35000\">%d</span>", success_count_words);
    label_success_words = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_success_words), bufer);
    gtk_label_set_justify(GTK_LABEL(label_success_words), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(tr_box), label_success_words, TRUE, TRUE, 30);

    sprintf(bufer, "<span size=\"15000\">Процент правильных ответов</span>\n<span size=\"35000\">%.1f%%</span>", pecent_words);
    label_pecent = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label_pecent), 0.5);
    gtk_label_set_markup(GTK_LABEL(label_pecent), bufer);
    gtk_label_set_justify(GTK_LABEL(label_pecent), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(tr_box), label_pecent, TRUE, TRUE, 30);

    btn_to_menu = gtk_button_new_with_label("В главное меню");
    g_signal_connect(G_OBJECT(btn_to_menu), "clicked", gtk_main_quit, NULL);
    gtk_box_pack_start(GTK_BOX(tr_box), btn_to_menu, FALSE, FALSE, 0);

    g_free(bufer);
    gtk_widget_show_all(tr_box);
    gtk_main();
    gtk_main_quit();
}

char* settime(struct tm* u)
{
    char s[40];
    char* tmp;
    for (int i = 0; i < 40; i++)
        s[i] = 0;
    int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S", u);
    tmp = (char*)malloc(sizeof(s));
    strcpy(tmp, s);
    return (tmp);
}