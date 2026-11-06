/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdint.h>

#include <sparkcli.h>

#include "capture.h"

static char output[4096];

static const uint8_t SAMPLE[] = {0x53, 0x70, 0x61, 0x72};

/**
 * The render loop used to be a do/while, so a zero length buffer still drew
 * one row: an offset of 00000000 followed by a full line of blank hex columns.
 */
static void test_empty_buffer_prints_nothing(void **state)
{
    (void)state;

    capture_start();
    scli_hexdump8(SAMPLE, 0);
    scli_hexdump16(SAMPLE, 0);
    scli_hexdump32(SAMPLE, 0);
    scli_hexdump(SAMPLE, 0, 4);
    assert_int_equal(capture_stop(output, sizeof(output)), 0);

    capture_start();
    scli_hex8(SAMPLE, 0);
    scli_hex16(SAMPLE, 0);
    scli_hex32(SAMPLE, 0);
    scli_hex(SAMPLE, 0, 4);
    assert_int_equal(capture_stop(output, sizeof(output)), 0);
}

/**
 * A chunk width of zero would advance the offset by nothing and loop forever.
 */
static void test_zero_chunk_width_terminates(void **state)
{
    (void)state;

    capture_start();
    scli_hexdump(SAMPLE, sizeof(SAMPLE), 0);
    scli_hex(SAMPLE, sizeof(SAMPLE), 0);
    assert_int_equal(capture_stop(output, sizeof(output)), 0);
}

/**
 * A non-empty buffer must still render exactly as before.
 */
static void test_non_empty_buffer_is_unaffected(void **state)
{
    (void)state;

    capture_start();
    scli_hexdump16(SAMPLE, sizeof(SAMPLE));
    capture_stop(output, sizeof(output));

    size_t lines = 0;
    for (const char *c = output; *c; ++c)
        if (*c == '\n')
            ++lines;

    assert_int_equal(lines, 1);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_empty_buffer_prints_nothing),
        cmocka_unit_test(test_zero_chunk_width_terminates),
        cmocka_unit_test(test_non_empty_buffer_is_unaffected),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
