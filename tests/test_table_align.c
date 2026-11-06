/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sparkcli.h>

static scli_tbl_t *new_table(void)
{
    char *headers[] = {"a", "b", "c"};
    scli_tbl_t *table = scli_tbl_new(3, headers);
    assert_non_null(table);
    return table;
}

static void test_columns_start_left_aligned(void **state)
{
    (void)state;

    scli_tbl_t *table = new_table();
    for (size_t column = 0; column < table->column_count; ++column)
        assert_int_equal(table->alignments[column], SCLI_TBL_ALIGN_LEFT);
    scli_tbl_free(&table);
}

static void test_alignment_is_set_per_column(void **state)
{
    (void)state;

    scli_tbl_t *table = new_table();

    assert_true(scli_tbl_align(table, 0, SCLI_TBL_ALIGN_RIGHT));
    assert_true(scli_tbl_align(table, 2, SCLI_TBL_ALIGN_CENTER));

    assert_int_equal(table->alignments[0], SCLI_TBL_ALIGN_RIGHT);
    assert_int_equal(table->alignments[1], SCLI_TBL_ALIGN_LEFT);
    assert_int_equal(table->alignments[2], SCLI_TBL_ALIGN_CENTER);

    scli_tbl_free(&table);
}

static void test_out_of_range_column_is_rejected(void **state)
{
    (void)state;

    scli_tbl_t *table = new_table();

    assert_false(scli_tbl_align(table, 3, SCLI_TBL_ALIGN_RIGHT));
    assert_false(scli_tbl_align(table, 99, SCLI_TBL_ALIGN_RIGHT));
    assert_false(scli_tbl_align(NULL, 0, SCLI_TBL_ALIGN_RIGHT));

    scli_tbl_free(&table);
}

/**
 * Rendering an aligned table must stay well behaved for cells both shorter
 * and longer than the column, which is where the padding arithmetic lives.
 */
static void test_render_with_alignment(void **state)
{
    (void)state;

    char *headers[] = {"#", "name"};
    char *row[] = {"1", "a-much-longer-value"};
    scli_tbl_t *table = scli_tbl_new(2, headers);
    assert_non_null(table);

    assert_true(scli_tbl_align(table, 0, SCLI_TBL_ALIGN_RIGHT));
    assert_true(scli_tbl_align(table, 1, SCLI_TBL_ALIGN_CENTER));
    assert_true(scli_tbl_add_row(table, row));

    scli_tbl_render(table);

    scli_tbl_free(&table);
    assert_null(table);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_columns_start_left_aligned),
        cmocka_unit_test(test_alignment_is_set_per_column),
        cmocka_unit_test(test_out_of_range_column_is_rejected),
        cmocka_unit_test(test_render_with_alignment),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
