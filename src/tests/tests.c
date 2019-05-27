#include "ctest.h"

CTEST(string, test1)
{
    ASSERT_STR("foo", "foo");
}

CTEST(string, test2)
{
    ASSERT_STR("qq", "foo");
}

CTEST(integer, test1)
{
    ASSERT_EQUAL(1, 2);
}

CTEST(integer, test2)
{
    ASSERT_EQUAL(2, 2);
}
