#include "sv.h"
#include "rktest.h"
#include <stdint.h>

// CREATION

TEST(creation_tests, sv_from_cstr)
{
    char *test_str = "something";
    uint8_t test_str_len = strlen(test_str);

    StringView test_sv = sv_from_cstr(test_str);
    EXPECT_EQ(test_sv.len, test_str_len);
    for (uint8_t i = 0; i < test_str_len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], test_sv.data[i]);
    }
}

TEST(creation_tests, sv_construct)
{
    char *test_str = "something";
    uint8_t test_str_len = strlen(test_str);
    StringView test_sv = sv_construct(test_str, test_str_len);
    EXPECT_EQ(test_sv.len, test_str_len);
    for (uint8_t i = 0; i < test_str_len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], test_sv.data[i]);
    }
}

// UTILITY

TEST(utility_tests, read_file_cstr)
{
    char *text = read_file_cstr("./tests/test_files/utility_test_file.txt");
    EXPECT_STREQ(text, "test text\r\n");
    free(text);
}

TEST(utility_tests, macro_StringViewFromStr)
{
    char *test_str = "something";
    uint8_t test_str_len = strlen(test_str);
    StringView test_sv = StringViewFromStr(test_str);

    EXPECT_EQ(test_sv.len, test_str_len);
    for (uint8_t i = 0; i < test_str_len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], test_sv.data[i]);
    }
}

TEST(utility_tests, macro_StringViewNull)
{
    char *test_str = "something";
    uint8_t test_str_len = strlen(test_str);
    StringView test_sv = StringViewFromStr(test_str);

    EXPECT_EQ(test_sv.len, test_str_len);
    for (uint8_t i = 0; i < test_str_len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], test_sv.data[i]);
    }
}

TEST(compare_tests, sv_compare__equals)
{
    StringView test_sv_a = StringViewFromStr("something");
    StringView test_sv_b = StringViewFromStr("something");
    EXPECT_TRUE(sv_compare(test_sv_a, test_sv_b));
}

TEST(compare_tests, sv_compare__not_equals)
{
    StringView test_sv_a = StringViewFromStr("something");
    StringView test_sv_c = StringViewFromStr("nothing");
    EXPECT_FALSE(sv_compare(test_sv_a, test_sv_c));
}

TEST(compare_tests, sv_compare__with_empty)
{
    StringView test_sv_a = StringViewFromStr("something");
    StringView test_sv_c = StringViewFromStr("");
    EXPECT_FALSE(sv_compare(test_sv_a, test_sv_c));
}

TEST(compare_tests, sv_compare__with_null)
{
    StringView test_sv_a = StringViewFromStr("something");
    StringView test_sv_c = StringViewNull;
    EXPECT_FALSE(sv_compare(test_sv_a, test_sv_c));
}

TEST(compare_tests, sv_compare__null_with_null)
{
    StringView test_sv_a = StringViewNull;
    StringView test_sv_c = StringViewNull;
    EXPECT_TRUE(sv_compare(test_sv_a, test_sv_c));
}

TEST(compare_tests, sv_compare__empty_with_null)
{
    StringView test_sv_a = StringViewFromStr("");
    StringView test_sv_c = StringViewNull;
    EXPECT_TRUE(sv_compare(test_sv_a, test_sv_c));
}

TEST(compare_tests, sv_compare__empty_with_empty)
{
    StringView test_sv_a = StringViewFromStr("");
    StringView test_sv_c = StringViewFromStr("");
    EXPECT_TRUE(sv_compare(test_sv_a, test_sv_c));
}

// SPLITS

TEST(split_tests, sv_split_left__existing_middle)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_left(&test_sv, 't');

    EXPECT_EQ(test_sv.len, 4);
    EXPECT_EQ(left.len, 4);

    char *result_str = "some";
    for (uint8_t i = 0; i < left.len; i++)
    {
        EXPECT_CHAR_EQ(result_str[i], left.data[i]);
    }

    char *remainder_str = "hing";
    for (uint8_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(remainder_str[i], test_sv.data[i]);
    }
}

TEST(split_tests, sv_split_left__existing_first)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_left(&test_sv, 's');

    EXPECT_EQ(test_sv.len, 8);
    EXPECT_EQ(left.len, 0);

    char *remainder_str = "omething";
    for (uint8_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(remainder_str[i], test_sv.data[i]);
    }
}

TEST(split_tests, sv_split_left__existing_last)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_left(&test_sv, 'g');

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 8);

    char *left_str = "somethin";
    for (uint8_t i = 0; i < left.len; i++)
    {
        EXPECT_CHAR_EQ(left_str[i], left.data[i]);
    }
}

TEST(split_tests, sv_split_left__non_existing)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_left(&test_sv, 'z');

    EXPECT_EQ(test_sv.len, 9);
    EXPECT_EQ(left.len, 9);

    char *left_str = "something";
    for (uint8_t i = 0; i < left.len; i++)
    {
        EXPECT_CHAR_EQ(left_str[i], left.data[i]);
    }
}

TEST(split_tests, sv_split_left__null_string_view)
{
    StringView test_sv = StringViewNull;
    StringView left = sv_split_left(&test_sv, 'z');

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 0);
}

TEST(split_tests, sv_split_left__empty_string_view)
{
    StringView test_sv = StringViewFromStr("");
    StringView left = sv_split_left(&test_sv, 'z');

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 0);
}

TEST(split_tests, sv_split_right__existing_middle)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_right(&test_sv, 't');

    EXPECT_EQ(test_sv.len, 4);
    EXPECT_EQ(left.len, 4);

    char *result_str = "hing";
    for (uint8_t i = 0; i < left.len; i++)
    {
        EXPECT_CHAR_EQ(result_str[i], left.data[i]);
    }

    char *remainder_str = "some";
    for (uint8_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(remainder_str[i], test_sv.data[i]);
    }
}

TEST(split_tests, sv_split_right__existing_first)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_right(&test_sv, 's');

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 8);

    char *left_str = "omething";
    for (uint8_t i = 0; i < left.len; i++)
    {
        EXPECT_CHAR_EQ(left_str[i], left.data[i]);
    }
}

TEST(split_tests, sv_split_right__existing_last)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_right(&test_sv, 'g');

    EXPECT_EQ(test_sv.len, 8);
    EXPECT_EQ(left.len, 0);

    char *remainder_str = "somethin";
    for (uint8_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(remainder_str[i], test_sv.data[i]);
    }
}

TEST(split_tests, sv_split_right__non_existing)
{
    StringView test_sv = StringViewFromStr("something");
    StringView left = sv_split_right(&test_sv, 'z');

    EXPECT_EQ(test_sv.len, 9);
    EXPECT_EQ(left.len, 0);

    char *remainder_str = "something";
    for (uint8_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(remainder_str[i], test_sv.data[i]);
    }
}

TEST(split_tests, sv_split_right__null_string_view)
{
    StringView test_sv = StringViewNull;
    StringView left = sv_split_right(&test_sv, 'z');

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 0);
}

TEST(split_tests, sv_split_right__empty_string_view)
{
    StringView test_sv = StringViewFromStr("");
    StringView left = sv_split_right(&test_sv, 'z');

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 0);
}

// CUTS

TEST(cut_tests, sv_cut_left__middle)
{
    char *test_str = "something";
    size_t n = 3;
    StringView test_sv = StringViewFromStr(test_str);

    StringView left = sv_cut_left(&test_sv, n);

    EXPECT_EQ(test_sv.len, strlen(test_str) - n);
    EXPECT_EQ(left.len, n);

    for (size_t i = 0; i < n; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], left.data[i]);
    }

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + n], test_sv.data[i]);
    }
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("ething")));
    EXPECT_TRUE(sv_compare(left, StringViewFromStr("som")));
}

TEST(cut_tests, sv_cut_left__start)
{
    char *test_str = "something";
    size_t n = 1;
    StringView test_sv = StringViewFromStr(test_str);

    StringView left = sv_cut_left(&test_sv, n);

    EXPECT_EQ(test_sv.len, strlen(test_str) - n);
    EXPECT_EQ(left.len, n);

    for (size_t i = 0; i < n; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], left.data[i]);
    }

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + n], test_sv.data[i]);
    }

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("omething")));
    EXPECT_TRUE(sv_compare(left, StringViewFromStr("s")));
}

TEST(cut_tests, sv_cut_left__end)
{
    char *test_str = "something";
    size_t n = 8;
    StringView test_sv = StringViewFromStr(test_str);

    StringView left = sv_cut_left(&test_sv, n);

    EXPECT_EQ(test_sv.len, 1);
    EXPECT_EQ(left.len, 8);

    for (size_t i = 0; i < n; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], left.data[i]);
    }

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + n], test_sv.data[i]);
    }

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("g")));
    EXPECT_TRUE(sv_compare(left, StringViewFromStr("somethin")));
}

TEST(cut_tests, sv_cut_left__zero)
{
    char *test_str = "something";
    size_t n = 0;
    StringView test_sv = StringViewFromStr(test_str);

    StringView left = sv_cut_left(&test_sv, n);

    EXPECT_EQ(test_sv.len, strlen(test_str));
    EXPECT_EQ(left.len, 0);

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + n], test_sv.data[i]);
    }

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("something")));
    EXPECT_TRUE(sv_compare(left, StringViewFromStr("")));
}

TEST(cut_tests, sv_cut_left__cut_empty)
{
    char *test_str = "";
    size_t n = 0;
    StringView test_sv = StringViewFromStr(test_str);

    StringView left = sv_cut_left(&test_sv, n);

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 0);

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
    EXPECT_TRUE(sv_compare(left, StringViewNull));
}

TEST(cut_tests, sv_cut_left__cut_greater)
{
    char *test_str = "abcdef";
    size_t n = 7;
    StringView test_sv = StringViewFromStr(test_str);

    StringView left = sv_cut_left(&test_sv, n);

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(left.len, 6);

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
    EXPECT_TRUE(sv_compare(left, StringViewFromStr("abcdef")));
}

TEST(cut_tests, sv_cut_right__middle)
{
    char *test_str = "something";
    size_t n = 3;
    StringView test_sv = StringViewFromStr(test_str);

    StringView right = sv_cut_right(&test_sv, n);

    EXPECT_EQ(test_sv.len, 6);
    EXPECT_EQ(right.len, 3);

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], test_sv.data[i]);
    }

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("someth")));
    EXPECT_TRUE(sv_compare(right, StringViewFromStr("ing")));
}

TEST(cut_tests, sv_cut_right__start)
{
    char *test_str = "something";
    size_t n = 1;
    StringView test_sv = StringViewFromStr(test_str);

    StringView right = sv_cut_right(&test_sv, n);

    EXPECT_EQ(test_sv.len, 8);
    EXPECT_EQ(right.len, 1);

    for (size_t i = 0; i < n; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + 8], right.data[i]);
    }

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], test_sv.data[i]);
    }
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("somethin")));
    EXPECT_TRUE(sv_compare(right, StringViewFromStr("g")));
}

TEST(cut_tests, sv_cut_right__end)
{
    char *test_str = "something";
    size_t n = 9;
    StringView test_sv = StringViewFromStr(test_str);

    StringView right = sv_cut_right(&test_sv, 9);

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(right.len, 9);

    for (size_t i = 0; i < n; i++)
    {
        EXPECT_CHAR_EQ(test_str[i], right.data[i]);
    }

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + n], test_sv.data[i]);
    }
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
    EXPECT_TRUE(sv_compare(right, StringViewFromStr("something")));
}

TEST(cut_tests, sv_cut_right__zero)
{
    char *test_str = "something";
    size_t n = 0;
    StringView test_sv = StringViewFromStr(test_str);

    StringView right = sv_cut_right(&test_sv, n);

    EXPECT_EQ(test_sv.len, 9);
    EXPECT_EQ(right.len, 0);

    for (size_t i = 0; i < test_sv.len; i++)
    {
        EXPECT_CHAR_EQ(test_str[i + n], test_sv.data[i]);
    }

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("something")));
    EXPECT_TRUE(sv_compare(right, StringViewFromStr("")));
}

TEST(cut_tests, sv_cut_right__cut_empty)
{
    char *test_str = "";
    size_t n = 0;
    StringView test_sv = StringViewFromStr(test_str);

    StringView right = sv_cut_right(&test_sv, n);

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(right.len, 0);

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
    EXPECT_TRUE(sv_compare(right, StringViewNull));
}

TEST(cut_tests, sv_cut_right__cut_greater)
{
    char *test_str = "abcdef";
    size_t n = 7;
    StringView test_sv = StringViewFromStr(test_str);

    StringView right = sv_cut_right(&test_sv, n);

    EXPECT_EQ(test_sv.len, 0);
    EXPECT_EQ(right.len, 6);

    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
    EXPECT_TRUE(sv_compare(right, StringViewFromStr("abcdef")));
}

// PREDICATES

TEST(predicates_tests, sv_whitespace_predicate)
{
    EXPECT_TRUE(sv_whitespace_predicate(' '));
    EXPECT_TRUE(sv_whitespace_predicate('\n'));
    EXPECT_TRUE(sv_whitespace_predicate('\r'));
    EXPECT_TRUE(sv_whitespace_predicate('\t'));
    EXPECT_FALSE(sv_whitespace_predicate('a'));
    EXPECT_FALSE(sv_whitespace_predicate(';'));
    EXPECT_FALSE(sv_whitespace_predicate('6'));
}

// FINDS

TEST(find_tests, sv_find_left_char__existing)
{
    StringView test_sv = StringViewFromStr("testing");

    EXPECT_EQ(sv_find_left_char(&test_sv, 't'), 0);
    EXPECT_EQ(sv_find_left_char(&test_sv, 's'), 2);
    EXPECT_EQ(sv_find_left_char(&test_sv, 'g'), 6);
}

TEST(find_tests, sv_find_left_char__non_existing)
{
    StringView test_sv = StringViewFromStr("testing");

    EXPECT_EQ(sv_find_left_char(&test_sv, 'z'), -1);
    EXPECT_EQ(sv_find_left_char(&test_sv, 'x'), -1);
    EXPECT_EQ(sv_find_left_char(&test_sv, 'f'), -1);
}

TEST(find_tests, sv_find_left_char__empty)
{
    StringView test_sv = StringViewFromStr("");

    EXPECT_EQ(sv_find_left_char(&test_sv, 'z'), -1);
    EXPECT_EQ(sv_find_left_char(&test_sv, 'x'), -1);
    EXPECT_EQ(sv_find_left_char(&test_sv, 'f'), -1);
}

TEST(find_tests, sv_find_right_char__existing)
{
    StringView test_sv = StringViewFromStr("testing");

    EXPECT_EQ(sv_find_right_char(&test_sv, 't'), 3);
    EXPECT_EQ(sv_find_right_char(&test_sv, 's'), 2);
    EXPECT_EQ(sv_find_right_char(&test_sv, 'g'), 6);
}

TEST(find_tests, sv_find_right_char__non_existing)
{
    StringView test_sv = StringViewFromStr("testing");

    EXPECT_EQ(sv_find_right_char(&test_sv, 'z'), -1);
    EXPECT_EQ(sv_find_right_char(&test_sv, 'x'), -1);
    EXPECT_EQ(sv_find_right_char(&test_sv, 'f'), -1);
}

TEST(find_tests, sv_find_right_char__empty)
{
    StringView test_sv = StringViewFromStr("");

    EXPECT_EQ(sv_find_right_char(&test_sv, 'z'), -1);
    EXPECT_EQ(sv_find_right_char(&test_sv, 'x'), -1);
    EXPECT_EQ(sv_find_right_char(&test_sv, 'f'), -1);
}

bool _predicate_foo(char c)
{
    return c == 'c';
};

TEST(find_tests, sv_find_left_predicate__existing)
{
    StringView test_sv = StringViewFromStr("abcdef");
    int idx = sv_find_left_predicate(&test_sv, _predicate_foo);
    EXPECT_EQ(idx, 2);
}


bool _predicate__false_foo(char c)
{
    return false;
};

TEST(find_tests, sv_find_left_predicate__non_existing)
{
    StringView test_sv = StringViewFromStr("abcdef");
    int idx = sv_find_left_predicate(&test_sv, _predicate__false_foo);
    EXPECT_EQ(idx, -1);
}

//STARTS WITH

TEST(starts_tests, sv_starts_with__equal)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("abcdef");
    EXPECT_TRUE(sv_starts_with(test_sv, other_sv));
}

TEST(starts_tests, sv_starts_with__shorter)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("abc");
    EXPECT_TRUE(sv_starts_with(test_sv, other_sv));
}

TEST(starts_tests, sv_starts_with__longer)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("abcefgh");
    EXPECT_FALSE(sv_starts_with(test_sv, other_sv));
}

TEST(starts_tests, sv_starts_with__different)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("zxc");
    EXPECT_FALSE(sv_starts_with(test_sv, other_sv));
}

TEST(starts_tests, sv_starts_with__fraction)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("bcd");
    EXPECT_FALSE(sv_starts_with(test_sv, other_sv));
}

TEST(starts_tests, sv_starts_with__empty)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("");
    EXPECT_TRUE(sv_starts_with(test_sv, other_sv));
}

TEST(starts_tests, sv_starts_with__both_empty)
{
    StringView test_sv = StringViewFromStr("");
    StringView other_sv = StringViewFromStr("");
    EXPECT_TRUE(sv_starts_with(test_sv, other_sv));
}

bool _alphanum_prediucate(char n)
{
    return (isalnum(n) > 0);
}

TEST(starts_tests, sv_starts_with_predicate__detect_all)
{
    StringView test_sv = StringViewFromStr("asdfg");
    int count = sv_starts_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 5);
}

TEST(starts_tests, sv_starts_with_predicate__detect_few)
{
    StringView test_sv = StringViewFromStr("as-+=");
    int count = sv_starts_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 2);
}

TEST(starts_tests, sv_starts_with_predicate__detect_none)
{
    StringView test_sv = StringViewFromStr("-+=!@");
    int count = sv_starts_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

TEST(starts_tests, sv_starts_with_predicate__empty)
{
    StringView test_sv = StringViewFromStr("");
    int count = sv_starts_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

TEST(starts_tests, sv_starts_with_predicate__null)
{
    StringView test_sv = StringViewNull;
    int count = sv_starts_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

TEST(starts_tests, sv_starts_with_predicate__not_detecting_in_middle)
{
    StringView test_sv = StringViewFromStr("+=asdf");
    int count = sv_starts_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

//ENDS WITH

TEST(ends_tests, sv_ends_with__equal)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("abcdef");
    EXPECT_TRUE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with__shorter)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("def");
    EXPECT_TRUE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with__longer)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("abcefgh");
    EXPECT_FALSE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with__different)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("zxc");
    EXPECT_FALSE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with__fraction)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("bcd");
    EXPECT_FALSE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with__empty)
{
    StringView test_sv = StringViewFromStr("abcdef");
    StringView other_sv = StringViewFromStr("");
    EXPECT_TRUE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with__both_empty)
{
    StringView test_sv = StringViewFromStr("");
    StringView other_sv = StringViewFromStr("");
    EXPECT_TRUE(sv_ends_with(test_sv, other_sv));
}

TEST(ends_tests, sv_ends_with_predicate__detect_all)
{
    StringView test_sv = StringViewFromStr("asdfg");
    int count = sv_ends_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 5);
}

TEST(ends_tests, sv_ends_with_predicate__detect_few)
{
    StringView test_sv = StringViewFromStr("-+=as");
    int count = sv_ends_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 2);
}

TEST(ends_tests, sv_ends_with_predicate__detect_none)
{
    StringView test_sv = StringViewFromStr("-+=!@");
    int count = sv_ends_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

TEST(ends_tests, sv_ends_with_predicate__empty)
{
    StringView test_sv = StringViewFromStr("");
    int count = sv_ends_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

TEST(ends_tests, sv_ends_with_predicate__null)
{
    StringView test_sv = StringViewNull;
    int count = sv_ends_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

TEST(ends_tests, sv_ends_with_predicate__not_detecting_in_middle)
{
    StringView test_sv = StringViewFromStr("asdf+=");
    int count = sv_ends_with_predicate(&test_sv, _alphanum_prediucate);

    EXPECT_EQ(count, 0);
}

//STRIPS 

TEST(strips_tests, sv_strip_left__existing)
{
    StringView test_sv = StringViewFromStr(" \nasdf");
    int count = sv_strip_left(&test_sv);

    EXPECT_EQ(count, 2);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("asdf")));
}

TEST(strips_tests, sv_strip_left__non_existing)
{
    StringView test_sv = StringViewFromStr("asdf");
    int count = sv_strip_left(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("asdf")));
}

TEST(strips_tests, sv_strip_left__does_not_strip_from_middle)
{
    StringView test_sv = StringViewFromStr("as      df");
    int count = sv_strip_left(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("as      df")));
}

TEST(strips_tests, sv_strip_left__empty)
{
    StringView test_sv = StringViewFromStr("");
    int count = sv_strip_left(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
}

TEST(strips_tests, sv_strip_left__null)
{
    StringView test_sv = StringViewNull;
    int count = sv_strip_left(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewNull));
}

TEST(strips_tests, sv_strip_right__existing)
{
    StringView test_sv = StringViewFromStr("asdf \n");
    int count = sv_strip_right(&test_sv);

    EXPECT_EQ(count, 2);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("asdf")));
}

TEST(strips_tests, sv_strip_right__non_existing)
{
    StringView test_sv = StringViewFromStr("asdf");
    int count = sv_strip_right(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("asdf")));
}

TEST(strips_tests, sv_strip_right__does_not_strip_from_middle)
{
    StringView test_sv = StringViewFromStr("as      df");
    int count = sv_strip_right(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("as      df")));
}

TEST(strips_tests, sv_strip_right__empty)
{
    StringView test_sv = StringViewFromStr("");
    int count = sv_strip_right(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewFromStr("")));
}

TEST(strips_tests, sv_strip_right__null)
{
    StringView test_sv = StringViewNull;
    int count = sv_strip_right(&test_sv);

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(sv_compare(test_sv, StringViewNull));
}