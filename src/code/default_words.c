#include "default_words.h"
FILE* def_words(FILE* file_voc)
{
    struct Item default_words[NUM_DEF_WORDS] = {{"bed", "кровать"},     {"book", "книга"},          {"branch", "ветвь"},           {"fire", "огонь"},       {"group", "группа"},
                                                {"hate", "ненавидеть"}, {"insurance", "страховка"}, {"knowledge", "знание"},       {"meal", "еда"},         {"noise", "шум"},
                                                {"opinion", "мнение"},  {"owner", "владелец"},      {"print", "печатать"},         {"quality", "качество"}, {"ray", "луч"},
                                                {"river", "река"},      {"smile", "улыбка"},        {"suggestion", "предложение"}, {"swim", "плавать"},     {"trouble", "проблема"},
                                                {"wool", "шерсть"},     {"wind", "ветер"},          {"weather", "погода"},         {"trade", "торговля"},   {"tax", "налог"},
                                                {"talk", "говорить"},   {"stone", "камень"},        {"statement", "утверждение"},  {"smoke", "дым"},        {"sleep", "спать"}};

    fwrite(default_words,sizeof(struct Item)*NUM_DEF_WORDS,1,file_voc);
    return file_voc;
}