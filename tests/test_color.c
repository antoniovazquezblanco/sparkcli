/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

#include <sparkcli.h>

#include "utils/env.h"

#ifdef _WIN32
#define set_env(name, value) _putenv_s(name, value)
#define clear_env(name) _putenv_s(name, "")
#else
#define set_env(name, value) setenv(name, value, 1)
#define clear_env(name) unsetenv(name)
#endif

/**
 * NO_COLOR keys off presence rather than off a value: any non-empty content
 * disables color, while an empty variable is ignored.
 *
 * This exercises scli_env_is_set() directly rather than scli_ses_color(). Under a
 * test runner the standard output is a pipe, so color is already off for want
 * of a terminal and asserting on it would pass whether NO_COLOR is honoured
 * or not.
 */
static void test_presence_not_value(void **state)
{
    (void)state;

    set_env("SCLI_TEST_NO_COLOR", "1");
    assert_true(scli_env_is_set("SCLI_TEST_NO_COLOR"));

    /* The convention disables color for any value, "0" included. */
    set_env("SCLI_TEST_NO_COLOR", "0");
    assert_true(scli_env_is_set("SCLI_TEST_NO_COLOR"));

    set_env("SCLI_TEST_NO_COLOR", "false");
    assert_true(scli_env_is_set("SCLI_TEST_NO_COLOR"));
}

static void test_empty_and_absent_are_not_set(void **state)
{
    (void)state;

    set_env("SCLI_TEST_NO_COLOR", "");
    assert_false(scli_env_is_set("SCLI_TEST_NO_COLOR"));

    clear_env("SCLI_TEST_NO_COLOR");
    assert_false(scli_env_is_set("SCLI_TEST_NO_COLOR"));

    assert_false(scli_env_is_set("SCLI_TEST_NEVER_DEFINED"));
}

/**
 * With NO_COLOR in effect the reported color system must be NONE whatever the
 * terminal claims to support.
 */
static void test_no_color_forces_color_system_none(void **state)
{
    (void)state;

    set_env("COLORTERM", "truecolor");
    set_env("NO_COLOR", "1");

    assert_int_equal(scli_color_system(), COLOR_SYSTEM_NONE);
    assert_false(scli_ses_color());
    assert_false(scli_ses_truecolor());

    clear_env("NO_COLOR");
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_presence_not_value),
        cmocka_unit_test(test_empty_and_absent_are_not_set),
        cmocka_unit_test(test_no_color_forces_color_system_none),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
