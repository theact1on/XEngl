#include "training.h"
#include "checks.h"
#include "default_words.h"
#include "stats_win.h"

void detect_info_bar(GtkWidget* widget, gpointer data)
{
    if (GTK_IS_INFO_BAR(widget) && *(int*)data == 0) {
        *(int*)data = 1;
    } else if (GTK_IS_INFO_BAR(widget)) {
        gtk_widget_destroy(widget);
        *(int*)data = 0;
    }
}

void training_win(GtkWidget* widget, gpointer data)
{
    srand(time(NULL));

    int num_of_words = count_words();
    int all_count_words = 0;
    int success_count_words = 0;
    int breakout = 0;
    int info_bar_showed = 0;
    task_function types_of_task[2] = {four_buttons_task, enter_translate_task};
    GtkWidget* window = (GtkWidget*)data;
    GtkWidget *tr_box, *label_name_window, *task_box, *button_box, *btn_next, *btn_end;
    GtkWidget* main_overlay = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(window)));
    GtkWidget* main_box = g_object_ref((GtkWidget*)gtk_bin_get_child(GTK_BIN(main_overlay)));
    GtkWidget* info_bar = gtk_info_bar_new();
    gtk_widget_set_name(info_bar, "error_bar");
    GtkWidget* label_error = gtk_label_new("");
    GtkWidget* content_area = gtk_info_bar_get_content_area(GTK_INFO_BAR(info_bar));
    g_signal_connect(info_bar, "response", G_CALLBACK(gtk_widget_destroy), NULL);
    gtk_container_add(GTK_CONTAINER(content_area), label_error);
    gtk_info_bar_set_show_close_button(GTK_INFO_BAR(info_bar), TRUE);
    gtk_widget_set_valign(info_bar, GTK_ALIGN_START);
    gtk_info_bar_set_revealed(GTK_INFO_BAR(info_bar), FALSE);

    gtk_container_forall(GTK_CONTAINER(main_overlay), detect_info_bar, (gpointer)&info_bar_showed);

    if (num_of_words == 0) {
        breakout = 1;
        if (!info_bar_showed) {
            gtk_label_set_text(GTK_LABEL(label_error), "Словарь пуст");
            gtk_info_bar_set_message_type(GTK_INFO_BAR(info_bar), GTK_MESSAGE_ERROR);
            gtk_overlay_add_overlay(GTK_OVERLAY(main_overlay), info_bar);
        }
    } else if (info_bar_showed == 1) {
        gtk_container_forall(GTK_CONTAINER(main_overlay), detect_info_bar, (gpointer)&info_bar_showed);
    }

    gtk_container_remove(GTK_CONTAINER(main_overlay), main_box);
    tr_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(tr_box, "tr_box");
    gtk_container_add(GTK_CONTAINER(main_overlay), tr_box);

    label_name_window = gtk_label_new(NULL);
    gtk_widget_set_name(label_name_window, "header");
    gtk_label_set_markup(GTK_LABEL(label_name_window), "Тренировка");
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
        types_of_task[rand_type_task](GTK_BOX(task_box), num_of_words, &success_count_words, btn_end, btn_next, &breakout);
        gtk_widget_set_sensitive(btn_end, FALSE);
        gtk_widget_set_sensitive(btn_next, FALSE);
        gtk_widget_show_all(tr_box);
        if (breakout == 2) {
            gtk_label_set_text(GTK_LABEL(label_error), "Не удалось открыть файл словаря во время тренировки");
            gtk_info_bar_set_message_type(GTK_INFO_BAR(info_bar), GTK_MESSAGE_ERROR);
            gtk_overlay_add_overlay(GTK_OVERLAY(main_overlay), info_bar);
            break;
        }
        gtk_main();
    }

    gtk_widget_destroy(tr_box);
    gtk_container_add(GTK_CONTAINER(main_overlay), main_box);
    gtk_widget_show_all(window);
    gtk_info_bar_set_revealed(GTK_INFO_BAR(info_bar), TRUE);
}

void four_buttons_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next, int* breakout)
{
    GQueue* list;
    list = g_queue_new();
    g_queue_push_head(list, success_count_words);
    g_queue_push_head(list, btn_end);
    g_queue_push_head(list, btn_next);

    srand(time(NULL));
    GtkWidget *label_word, *btn_answer_box, *answer_buttons[4], *scrolled_win;
    struct Item* its = (struct Item*)malloc(sizeof(struct Item));
    struct Item fail;

    label_word = gtk_label_new("LOADING...");
    scrolled_win = gtk_scrolled_window_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(scrolled_win), label_word);
    gtk_widget_set_margin_end(scrolled_win, 15);
    gtk_widget_set_margin_start(scrolled_win, 15);
    gtk_box_pack_start(task_box, scrolled_win, FALSE, FALSE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_win), 100);
    gtk_widget_set_name(scrolled_win, "scrolled_win_training");

    gtk_widget_set_name(label_word, "label_question");

    btn_answer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_name(btn_answer_box, "btn_train");
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
    if (question_word == NULL) {
        *breakout = 2;
        return;
    }
    fseek(question_word, rand_word[3] * sizeof(struct Item), 0);
    fread(its, sizeof(struct Item), 1, question_word);
    rand_word_item[3] = *its;

    g_queue_push_head(list, its);

    if (*type_its == 1) {
        gtk_label_set_text(GTK_LABEL(label_word), g_utf8_strup(its->word, -1));
        sprintf(bufer, "<span size=\"35000\">%s</span>", g_utf8_strup(its->word, -1));
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
        answer_buttons[3] = gtk_button_new_with_label(g_utf8_strup(its->translation, -1));
    } else {
        gtk_label_set_text(GTK_LABEL(label_word), g_utf8_strup(its->translation, -1));
        sprintf(bufer, "<span size=\"35000\">%s</span>", g_utf8_strup(its->translation, -1));
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
        answer_buttons[3] = gtk_button_new_with_label(g_utf8_strup(its->word, -1));
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
            answer_buttons[i - 4] = gtk_button_new_with_label(g_utf8_strup(fail.translation, -1));
        } else {
            answer_buttons[i - 4] = gtk_button_new_with_label(g_utf8_strup(fail.word, -1));
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

void enter_translate_task(GtkBox* task_box, int N_WORDS, int* success_count_words, GtkWidget* btn_end, GtkWidget* btn_next, int* breakout)
{
    GQueue* list;
    list = g_queue_new();
    g_queue_push_head(list, success_count_words);
    g_queue_push_head(list, btn_end);
    g_queue_push_head(list, btn_next);

    GtkWidget *label_word, *entry_answer_box, *entry_label, *button_success, *right_word, *scrolled_win_question, *scrolled_win_right_word;
    srand(time(NULL));

    label_word = gtk_label_new("LOADING...");
    scrolled_win_question = gtk_scrolled_window_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(scrolled_win_question), label_word);
    gtk_widget_set_margin_end(scrolled_win_question, 15);
    gtk_widget_set_margin_start(scrolled_win_question, 15);
    gtk_box_pack_start(task_box, scrolled_win_question, FALSE, FALSE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win_question), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_win_question), 100);
    gtk_widget_set_name(scrolled_win_question, "scrolled_win_training");

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
    int* type_its = (int*)malloc(sizeof(int));
    *type_its = rand() % 2 + 1;
    FILE* question_word = fopen("data/voc.dat", "rb");
    if (question_word == NULL) {
        *breakout = 2;
        return;
    }
    fseek(question_word, rand_word * sizeof(struct Item) + NUM_DEF_WORDS * sizeof(struct Item), 0);
    fread(its, sizeof(struct Item), 1, question_word);

    if (*type_its == 1) {
        gtk_label_set_text(GTK_LABEL(label_word), g_utf8_strup(its->word, -1));
        char bufer[130];
        sprintf(bufer, "<span size=\"35000\">%s</span>", g_utf8_strup(its->word, -1));
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
    } else {
        gtk_label_set_text(GTK_LABEL(label_word), g_utf8_strup(its->translation, -1));
        char bufer[130];
        sprintf(bufer, "<span size=\"35000\">%s</span>", g_utf8_strup(its->translation, -1));
        gtk_label_set_markup(GTK_LABEL(label_word), bufer);
    }

    fclose(question_word);

    g_queue_push_head(list, its);
    g_queue_push_head(list, type_its);
    g_signal_connect(G_OBJECT(entry_label), "activate", G_CALLBACK(enter_compare), list);
    g_signal_connect(G_OBJECT(button_success), "clicked", G_CALLBACK(enter_compare), list);
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