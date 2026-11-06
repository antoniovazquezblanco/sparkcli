/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdbool.h>

#include <sparkcli.h>

/*
 * A consumer is entitled to its own helpers with ordinary names. These used to
 * clash with SparkCLI's internals: calling into the library pulls the platform
 * console object out of the archive, and that object defined these same
 * symbols, so the link failed with a multiple definition error.
 *
 * This file is a link time test. If the library exports its internals again,
 * it stops building rather than stops passing.
 */
int console_getkey(void);
bool console_stdout_isatty(void);
bool console_stdin_isatty(void);
bool console_isdumb(void);
bool console_hastruecolor(void);
bool console_set_utf8_output(void);
bool env_tolower_matches(char *name, char *value);

int console_getkey(void) { return 'x'; }
bool console_stdout_isatty(void) { return false; }
bool console_stdin_isatty(void) { return false; }
bool console_isdumb(void) { return true; }
bool console_hastruecolor(void) { return false; }
bool console_set_utf8_output(void) { return false; }
bool env_tolower_matches(char *name, char *value) { (void)name; (void)value; return false; }

static void test_consumer_symbols_are_independent(void **state)
{
    (void)state;

    /* Links against SparkCLI's own internals, not the definitions above. */
    (void)scli_ses_interactive();
    (void)scli_color_system();

    assert_int_equal(console_getkey(), 'x');
    assert_true(console_isdumb());
    assert_false(console_stdout_isatty());
    assert_false(env_tolower_matches("TERM", "dumb"));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_consumer_symbols_are_independent),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
