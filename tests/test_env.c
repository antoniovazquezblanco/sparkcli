/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

#include "utils/env.h"

#ifdef _WIN32
#define set_env(name, value) _putenv_s(name, value)
#else
#define set_env(name, value) setenv(name, value, 1)
#endif

static void test_match_is_case_insensitive(void **state)
{
    (void)state;

    set_env("SCLI_TEST_VAR", "DuMb");
    assert_true(scli_env_tolower_matches("SCLI_TEST_VAR", "dumb"));

    set_env("SCLI_TEST_VAR", "dumb");
    assert_true(scli_env_tolower_matches("SCLI_TEST_VAR", "dumb"));

    set_env("SCLI_TEST_VAR", "smart");
    assert_false(scli_env_tolower_matches("SCLI_TEST_VAR", "dumb"));
}

static void test_missing_variable_does_not_match(void **state)
{
    (void)state;

    assert_false(scli_env_tolower_matches("SCLI_TEST_ABSENT_VAR", "anything"));
}

/**
 * tolower() is only defined for values representable as unsigned char, so a
 * byte over 0x7f must be cast before it is passed. Feed the lowercasing loop
 * such bytes and check it still reaches a correct answer.
 */
static void test_high_bytes_are_handled(void **state)
{
    (void)state;

    set_env("SCLI_TEST_VAR", "DUMB\xc3\xa9");
    assert_false(scli_env_tolower_matches("SCLI_TEST_VAR", "dumb"));
    assert_true(scli_env_tolower_matches("SCLI_TEST_VAR", "dumb\xc3\xa9"));

    set_env("SCLI_TEST_VAR", "\xff\xfe");
    assert_true(scli_env_tolower_matches("SCLI_TEST_VAR", "\xff\xfe"));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_match_is_case_insensitive),
        cmocka_unit_test(test_missing_variable_does_not_match),
        cmocka_unit_test(test_high_bytes_are_handled),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
