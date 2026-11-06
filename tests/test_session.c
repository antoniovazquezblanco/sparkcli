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

#ifdef _WIN32
#define set_env(name, value) _putenv_s(name, value)
#define clear_env(name) _putenv_s(name, "")
#else
#define set_env(name, value) setenv(name, value, 1)
#define clear_env(name) unsetenv(name)
#endif

/*
 * A test runner's standard output is a pipe rather than a terminal, so the
 * capability probes answer "no terminal" here. That is a weak assertion on its
 * own, so these tests check the relationships between the probes instead,
 * which have to hold whatever the environment looks like.
 */

/**
 * Truecolor is a stronger claim than color, and color is reported only when
 * the color system is something other than NONE.
 */
static void test_capabilities_are_consistent(void **state)
{
    (void)state;

    if (scli_ses_truecolor())
        assert_true(scli_ses_color());

    if (scli_ses_color())
        assert_int_not_equal(scli_color_system(), COLOR_SYSTEM_NONE);
    else
        assert_int_equal(scli_color_system(), COLOR_SYSTEM_NONE);
}

/**
 * A dumb terminal can never be interactive, nor report color.
 */
static void test_dumb_terminal_has_no_capabilities(void **state)
{
    (void)state;

    set_env("TERM", "dumb");
    assert_false(scli_ses_interactive());
    assert_false(scli_ses_color());
    assert_false(scli_ses_truecolor());

    set_env("TERM", "DUMB");
    assert_false(scli_ses_interactive());

    set_env("TERM", "unknown");
    assert_false(scli_ses_interactive());

    clear_env("TERM");
}

/**
 * Output redirected to a pipe is not a terminal, so the session cannot be
 * interactive no matter what COLORTERM advertises.
 */
static void test_piped_output_is_not_interactive(void **state)
{
    (void)state;

    set_env("COLORTERM", "truecolor");
    set_env("TERM", "xterm-256color");

    assert_false(scli_ses_interactive());

    clear_env("COLORTERM");
    clear_env("TERM");
}

static void test_probes_are_repeatable(void **state)
{
    (void)state;

    assert_int_equal(scli_ses_interactive(), scli_ses_interactive());
    assert_int_equal(scli_ses_color(), scli_ses_color());
    assert_int_equal(scli_color_system(), scli_color_system());
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_capabilities_are_consistent),
        cmocka_unit_test(test_dumb_terminal_has_no_capabilities),
        cmocka_unit_test(test_piped_output_is_not_interactive),
        cmocka_unit_test(test_probes_are_repeatable),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
