#include "checks.h"
#include "ctest.h"

CTEST(checkEnteredChar, correctCharacterAt0Position)
{
    ASSERT_EQUAL(0, check_valid_char(0, 'w'));
}
CTEST(checkEnteredChar, correctCharacter1)
{
    ASSERT_EQUAL(0, check_valid_char(33, ' '));
}
CTEST(checkEnteredChar, correctCharacter2)
{
    ASSERT_EQUAL(0, check_valid_char(2, 'g'));
}
CTEST(checkEnteredChar, invalidCharacterAt0Position)
{
    ASSERT_EQUAL(1, check_valid_char(0, ' '));
}
CTEST(checkEnteredChar, invalidCharacter1)
{
    ASSERT_EQUAL(1, check_valid_char(0, '1'));
}
CTEST(checkEnteredChar, invalidCharacter2)
{
    ASSERT_EQUAL(1, check_valid_char(33, '!'));
}

CTEST(compareWords, wordsEqual)
{
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));
    strcpy(item->word, "test");
    strcpy(item->translation, "теСт");
    ASSERT_EQUAL(1, compare_words("teST", item, 0));
}
CTEST(compareWords, translationsEqual)
{
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));
    strcpy(item->word, "teSt");
    strcpy(item->translation, "ТесТ");
    ASSERT_EQUAL(1, compare_words("тест", item, 1));
}
CTEST(compareWords, wordsNotEqual)
{
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));
    strcpy(item->word, "test");
    strcpy(item->translation, "тест");
    ASSERT_EQUAL(0, compare_words("tsEt", item, 0));
}
CTEST(compareWords, translationsNotEqual)
{
    struct Item* item;
    item = (struct Item*)malloc(sizeof(struct Item));
    strcpy(item->word, "test");
    strcpy(item->translation, "тест");
    ASSERT_EQUAL(0, compare_words("тсет", item, 1));
}

CTEST(compareStructs, structuresEqual1)
{
    struct Item *first, *second;
    first = (struct Item*)malloc(sizeof(struct Item));
    second = (struct Item*)malloc(sizeof(struct Item));
    strcpy(first->word, "test");
    strcpy(first->translation, "тесТ");
    strcpy(second->word, "TEST");
    strcpy(second->translation, "тЕст");
    ASSERT_EQUAL(1, compare_structs(first, second));
}
CTEST(compareStructs, structuresEqual2)
{
    struct Item *first, *second;
    first = (struct Item*)malloc(sizeof(struct Item));
    second = (struct Item*)malloc(sizeof(struct Item));
    strcpy(first->word, "test1");
    strcpy(first->translation, "тесТ");
    strcpy(second->word, "test2");
    strcpy(second->translation, "ТЕст");
    ASSERT_EQUAL(1, compare_structs(first, second));
}
CTEST(compareStructs, structuresNotEqual1)
{
    struct Item *first, *second;
    first = (struct Item*)malloc(sizeof(struct Item));
    second = (struct Item*)malloc(sizeof(struct Item));
    strcpy(first->word, "test");
    strcpy(first->translation, "тест1");
    strcpy(second->word, "test1");
    strcpy(second->translation, "тест");
    ASSERT_EQUAL(0, compare_structs(first, second));
}
CTEST(compareStructs, structuresNotEqual2)
{
    struct Item *first, *second;
    first = (struct Item*)malloc(sizeof(struct Item));
    second = (struct Item*)malloc(sizeof(struct Item));
    strcpy(first->word, "test1");
    strcpy(first->translation, ":CC");
    strcpy(second->word, "test2");
    strcpy(second->translation, ":с");
    ASSERT_EQUAL(0, compare_structs(first, second));
}
