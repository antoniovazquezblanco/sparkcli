/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <string.h>

#include <sparkcli.h>

#include "capture.h"

static char output[8192];

static void test_new_initialises_state(void **state)
{
    (void)state;

    capture_start();
    scli_prg_t *progress = scli_prg_new(50, "Rendering");
    capture_stop(output, sizeof(output));

    assert_non_null(progress);
    assert_int_equal(progress->total, 50);
    assert_int_equal(progress->current, 0);
    assert_string_equal(progress->label, "Rendering");

    scli_prg_free(progress);
}

/**
 * The label is duplicated, so a caller may reuse its buffer afterwards.
 */
static void test_label_is_copied(void **state)
{
    (void)state;

    char label[] = "Scanning";

    capture_start();
    scli_prg_t *progress = scli_prg_new(10, label);
    capture_stop(output, sizeof(output));

    label[0] = 'X';
    assert_string_equal(progress->label, "Scanning");

    scli_prg_free(progress);
}

static void test_update_tracks_progress(void **state)
{
    (void)state;

    capture_start();
    scli_prg_t *progress = scli_prg_new(10, "Work");
    scli_prg_update(progress, 5);
    capture_stop(output, sizeof(output));

    assert_int_equal(progress->current, 5);
    assert_non_null(strstr(output, "Work"));
    assert_non_null(strstr(output, "50%"));

    scli_prg_free(progress);
}

/**
 * Finishing must land on the total and report a complete bar, whatever the
 * last value passed to update was.
 */
static void test_finish_completes_the_bar(void **state)
{
    (void)state;

    capture_start();
    scli_prg_t *progress = scli_prg_new(8, "Work");
    scli_prg_update(progress, 3);
    scli_prg_finish(progress);
    capture_stop(output, sizeof(output));

    assert_int_equal(progress->current, 8);
    assert_non_null(strstr(output, "100%"));

    scli_prg_free(progress);
}

/**
 * A total of zero must not divide by zero, and an overshoot must clamp
 * rather than run past the end of the bar.
 */
static void test_degenerate_totals(void **state)
{
    (void)state;

    capture_start();
    scli_prg_t *zero = scli_prg_new(0, "Empty");
    scli_prg_update(zero, 0);
    scli_prg_finish(zero);

    scli_prg_t *over = scli_prg_new(5, "Over");
    scli_prg_update(over, 500);
    capture_stop(output, sizeof(output));

    assert_non_null(zero);
    assert_non_null(over);
    assert_non_null(strstr(output, "100%"));

    scli_prg_free(zero);
    scli_prg_free(over);
}

static void test_null_progress_is_tolerated(void **state)
{
    (void)state;

    scli_prg_update(NULL, 1);
    scli_prg_finish(NULL);
    scli_prg_free(NULL);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_initialises_state),
        cmocka_unit_test(test_label_is_copied),
        cmocka_unit_test(test_update_tracks_progress),
        cmocka_unit_test(test_finish_completes_the_bar),
        cmocka_unit_test(test_degenerate_totals),
        cmocka_unit_test(test_null_progress_is_tolerated),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
