/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>
#include <string.h>

#include "utils/str.h"

static void test_copy_is_independent_of_the_source(void **state)
{
    (void)state;

    char source[] = "Component";
    char *copy = str_dup(source);

    assert_non_null(copy);
    assert_string_equal(copy, "Component");
    assert_ptr_not_equal(copy, source);

    source[0] = 'X';
    assert_string_equal(copy, "Component");

    free(copy);
}

static void test_empty_string(void **state)
{
    (void)state;

    char *copy = str_dup("");
    assert_non_null(copy);
    assert_string_equal(copy, "");
    assert_int_equal(strlen(copy), 0);
    free(copy);
}

static void test_null_yields_null(void **state)
{
    (void)state;

    assert_null(str_dup(NULL));
}

/**
 * The whole string including the terminator has to be copied, so a long value
 * must survive intact rather than being truncated.
 */
static void test_long_string_is_copied_whole(void **state)
{
    (void)state;

    char source[512];
    memset(source, 'a', sizeof(source) - 1);
    source[sizeof(source) - 1] = '\0';

    char *copy = str_dup(source);
    assert_non_null(copy);
    assert_int_equal(strlen(copy), sizeof(source) - 1);
    assert_string_equal(copy, source);
    free(copy);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_copy_is_independent_of_the_source),
        cmocka_unit_test(test_empty_string),
        cmocka_unit_test(test_null_yields_null),
        cmocka_unit_test(test_long_string_is_copied_whole),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
