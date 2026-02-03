/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Kevin Leon
 */
#include "hexdump.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void _render_row(const uint8_t *buffer, size_t offset, size_t buffer_len, uint16_t chunk_len, bool addr_col, bool ascii_col)
{
    // Address column
    if (addr_col)
        printf("%08X | ", (unsigned int)offset);

    // Hex body
    for (size_t i = 0; i < chunk_len; i++)
    {
        size_t buf_off = offset + i;
        if (buf_off < buffer_len)
            printf("%02X ", buffer[buf_off]);
        else
            printf("   ");
    }

    // ASCII
    if (ascii_col)
    {
        printf("| ");
        for (size_t i = 0; i < chunk_len; i++)
        {
            size_t buf_off = offset + i;
            if (buf_off < buffer_len)
                printf("%c", (buffer[buf_off] >= 32 && buffer[buf_off] <= 126) ? buffer[buf_off] : '.');
            else
                printf(" ");
        }
    }

    printf("\n");
}

static void _render(const uint8_t *buffer, size_t buffer_len, uint16_t chunk_len, bool addr_col, bool ascii_col)
{
    size_t offset = 0;
    do
    {
        _render_row(buffer, offset, buffer_len, chunk_len, addr_col, ascii_col);
        offset += chunk_len;
    } while (offset < buffer_len);
}

void scli_hexdump(const uint8_t *buffer, size_t buffer_len, size_t chunk_len)
{
    _render(buffer, buffer_len, chunk_len, true, true);
}

void scli_hexdump8(const uint8_t *buffer, size_t buffer_len)
{
    scli_hexdump(buffer, buffer_len, 8);
}

void scli_hexdump16(const uint8_t *buffer, size_t buffer_len)
{
    scli_hexdump(buffer, buffer_len, 16);
}

void scli_hexdump32(const uint8_t *buffer, size_t buffer_len)
{
    scli_hexdump(buffer, buffer_len, 32);
}

void scli_hex(const uint8_t *buffer, size_t buffer_len, size_t chunk_len)
{
    _render(buffer, buffer_len, chunk_len, false, false);
}

void scli_hex8(const uint8_t *buffer, size_t buffer_len)
{
    scli_hex(buffer, buffer_len, 8);
}

void scli_hex16(const uint8_t *buffer, size_t buffer_len)
{
    scli_hex(buffer, buffer_len, 16);
}

void scli_hex32(const uint8_t *buffer, size_t buffer_len)
{
    scli_hex(buffer, buffer_len, 32);
}
