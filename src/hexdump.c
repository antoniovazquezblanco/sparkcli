/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Kevin Leon
 */
#include "hexdump.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK_32 (32)
#define CHUNK_16 (16)
#define CHUNK_8 (8)

static void _hexdump_render(const uint8_t *buffer, size_t buffer_len, uint16_t chunk_len)
{
    char ascii[chunk_len + 1];
    ascii[chunk_len] = '\0';

    for (size_t i = 0; i < buffer_len; i++)
    {
        if (i % chunk_len == 0)
        {
            if (i != 0)
            {
                printf("  |");
                printf("%s\n", ascii);
            }
            printf("%08X  ", (unsigned int)i);
        }

        printf("%02X ", buffer[i]);
        ascii[i % chunk_len] = (buffer[i] >= 32 && buffer[i] <= 126) ? buffer[i] : '.';
    }

    size_t remaining = buffer_len % chunk_len;
    if (remaining > 0)
        for (size_t i = remaining; i < chunk_len; i++)
            printf("   ");

    printf("  | ");
    ascii[remaining == 0 ? chunk_len : remaining] = '\0';
    printf("%s\n", ascii);
}

static void _hex_render(const uint8_t *buffer, size_t buffer_len, uint16_t chunk_len)
{
    for (int i = 0; i < buffer_len; ++i)
    {
        printf("%02X ", buffer[i]);
        if ((i + 1) % chunk_len == 0)
            printf("\n");
    }
    if ((buffer_len + 1) % chunk_len != 0)
        printf("\n");
}

void scli_hexdump(const uint8_t *buffer, size_t buffer_len, size_t chunk_len)
{
    _hexdump_render(buffer, buffer_len, chunk_len);
}

void scli_hexdump8(const uint8_t *buffer, size_t buffer_len)
{
    _hexdump_render(buffer, buffer_len, CHUNK_8);
}

void scli_hexdump16(const uint8_t *buffer, size_t buffer_len)
{
    _hexdump_render(buffer, buffer_len, CHUNK_16);
}

void scli_hexdump32(const uint8_t *buffer, size_t buffer_len)
{
    _hexdump_render(buffer, buffer_len, CHUNK_32);
}

void scli_hex(const uint8_t *buffer, size_t buffer_len, size_t chunk_len)
{
    _hex_render(buffer, buffer_len, chunk_len);
}

void scli_hex8(const uint8_t *buffer, size_t buffer_len)
{
    _hex_render(buffer, buffer_len, CHUNK_8);
}

void scli_hex16(const uint8_t *buffer, size_t buffer_len)
{
    _hex_render(buffer, buffer_len, CHUNK_16);
}

void scli_hex32(const uint8_t *buffer, size_t buffer_len)
{
    _hex_render(buffer, buffer_len, CHUNK_32);
}