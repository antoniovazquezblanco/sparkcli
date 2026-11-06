/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sparkcli.h>

static void test_level_round_trips(void **state)
{
    (void)state;

    scli_log_level_set(SCLI_LOGLEVEL_WARN);
    assert_int_equal(scli_log_level_get(), SCLI_LOGLEVEL_WARN);

    scli_log_level_set(SCLI_LOGLEVEL_DEBUG);
    assert_int_equal(scli_log_level_get(), SCLI_LOGLEVEL_DEBUG);

    scli_log_level_set(SCLI_LOGLEVEL_NONE);
    assert_int_equal(scli_log_level_get(), SCLI_LOGLEVEL_NONE);
}

/**
 * The levels have to stay ordered by severity, since the threshold check is a
 * comparison against them.
 */
static void test_levels_are_ordered_by_severity(void **state)
{
    (void)state;

    assert_true(SCLI_LOGLEVEL_DEBUG < SCLI_LOGLEVEL_INFO);
    assert_true(SCLI_LOGLEVEL_INFO < SCLI_LOGLEVEL_WARN);
    assert_true(SCLI_LOGLEVEL_WARN < SCLI_LOGLEVEL_ERROR);
    assert_true(SCLI_LOGLEVEL_ERROR < SCLI_LOGLEVEL_NONE);
}

/**
 * Logging below the threshold must be discarded rather than printed. The
 * calls below are all under the threshold, so nothing should be emitted.
 */
static void test_logging_below_the_threshold_is_discarded(void **state)
{
    (void)state;

    scli_log_level_set(SCLI_LOGLEVEL_NONE);
    scli_log_debug("debug %d", 1);
    scli_log_info("info %s", "x");
    scli_log_warn("warn");
    scli_log_error("error");

    assert_int_equal(scli_log_level_get(), SCLI_LOGLEVEL_NONE);

    scli_log_level_set(SCLI_LOGLEVEL_DEBUG);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_level_round_trips),
        cmocka_unit_test(test_levels_are_ordered_by_severity),
        cmocka_unit_test(test_logging_below_the_threshold_is_discarded),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
