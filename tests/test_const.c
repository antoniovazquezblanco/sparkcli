/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sparkcli.h>

/*
 * These are compile time assertions as much as run time ones. None of the
 * shapes below build against a char * API without casting away const, so a
 * regression shows up as a build failure rather than a test failure.
 */

typedef struct
{
    const char *name;
    const char *status;
} record_t;

/**
 * String literals, which is what a header row usually is.
 */
static void test_literals_need_no_scratch_buffers(void **state)
{
    (void)state;

    const char *const headers[] = {"Component", "Status"};
    const char *const row[] = {"Renderer", "Ready"};

    scli_tbl_t *table = scli_tbl_new(2, headers);
    assert_non_null(table);
    assert_true(scli_tbl_add_row(table, row));
    assert_string_equal(table->headers[0], "Component");
    assert_string_equal(table->cells[1], "Ready");

    scli_tbl_free(&table);
}

/**
 * Cells reached through a const struct, which previously could not be passed
 * at all without casting away const.
 */
static void test_cells_from_a_const_struct(void **state)
{
    (void)state;

    static const record_t records[] = {
        {"alpha", "ok"},
        {"beta", "degraded"},
    };

    const char *const headers[] = {"Name", "Status"};
    scli_tbl_t *table = scli_tbl_new(2, headers);
    assert_non_null(table);

    for (size_t i = 0; i < sizeof(records) / sizeof(records[0]); ++i)
    {
        const char *const row[] = {records[i].name, records[i].status};
        assert_true(scli_tbl_add_row(table, row));
    }

    assert_int_equal(table->row_count, 2);
    assert_string_equal(table->cells[0], "alpha");
    assert_string_equal(table->cells[3], "degraded");

    scli_tbl_free(&table);
}

/**
 * A progress label is duplicated immediately, so a literal must be accepted.
 */
static void test_progress_takes_a_const_label(void **state)
{
    (void)state;

    const char *label = "Scanning";
    scli_prg_t *progress = scli_prg_new(4, label);
    assert_non_null(progress);
    assert_string_equal(progress->label, "Scanning");

    scli_prg_update(progress, 2);
    scli_prg_finish(progress);
    scli_prg_free(progress);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_literals_need_no_scratch_buffers),
        cmocka_unit_test(test_cells_from_a_const_struct),
        cmocka_unit_test(test_progress_takes_a_const_label),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
