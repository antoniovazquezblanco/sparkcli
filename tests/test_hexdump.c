/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdint.h>
#include <string.h>

#include <sparkcli.h>

#include "capture.h"

static char output[16384];

static const uint8_t SAMPLE[] = {
    0x53, 0x70, 0x61, 0x72, 0x6b, 0x63, 0x6c, 0x69,
    0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33,
};

static size_t count_lines(const char *text)
{
    size_t lines = 0;
    for (const char *c = text; *c; ++c)
        if (*c == '\n')
            ++lines;
    return lines;
}

static void test_hexdump16_layout(void **state)
{
    (void)state;

    capture_start();
    scli_hexdump16(SAMPLE, sizeof(SAMPLE));
    capture_stop(output, sizeof(output));

    /* One line for sixteen bytes, carrying offset, hex and ASCII columns. */
    assert_int_equal(count_lines(output), 1);
    assert_non_null(strstr(output, "00000000"));
    assert_non_null(strstr(output, "53 70 61 72"));
    assert_non_null(strstr(output, "Sparkcli00112233"));
}

static void test_chunk_width_controls_line_count(void **state)
{
    (void)state;

    capture_start();
    scli_hexdump8(SAMPLE, sizeof(SAMPLE));
    capture_stop(output, sizeof(output));
    assert_int_equal(count_lines(output), 2);

    capture_start();
    scli_hexdump(SAMPLE, sizeof(SAMPLE), 4);
    capture_stop(output, sizeof(output));
    assert_int_equal(count_lines(output), 4);
}

/**
 * A trailing partial line must still be emitted rather than dropped.
 */
static void test_partial_final_line(void **state)
{
    (void)state;

    capture_start();
    scli_hexdump16(SAMPLE, 3);
    capture_stop(output, sizeof(output));

    assert_int_equal(count_lines(output), 1);
    assert_non_null(strstr(output, "53 70 61"));
    assert_non_null(strstr(output, "Spa"));
}

/**
 * scli_hex* prints the hex columns only, so the ASCII rendering of the
 * sample must not appear.
 */
static void test_hex_omits_the_ascii_column(void **state)
{
    (void)state;

    capture_start();
    scli_hex16(SAMPLE, sizeof(SAMPLE));
    capture_stop(output, sizeof(output));

    assert_non_null(strstr(output, "53 70 61 72"));
    assert_null(strstr(output, "Sparkcli00112233"));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hexdump16_layout),
        cmocka_unit_test(test_chunk_width_controls_line_count),
        cmocka_unit_test(test_partial_final_line),
        cmocka_unit_test(test_hex_omits_the_ascii_column),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
