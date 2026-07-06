/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "input.h"

#include "platform/console.h"

#include <stdio.h>
#include <stdlib.h>

int scli_inp_key(void)
{
    return console_getkey();
}

char *scli_inp_line(const char *prompt)
{
    if (prompt)
    {
        fputs(prompt, stdout);
        fflush(stdout);
    }

    size_t capacity = 64;
    size_t length = 0;
    char *buffer = malloc(capacity);
    if (!buffer)
        return NULL;

    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n')
    {
        if (length + 1 >= capacity)
        {
            size_t new_capacity = capacity * 2;
            char *grown = realloc(buffer, new_capacity);
            if (!grown)
            {
                free(buffer);
                return NULL;
            }
            buffer = grown;
            capacity = new_capacity;
        }
        buffer[length++] = (char)c;
    }

    // Nothing read and no newline consumed means we hit end-of-input.
    if (c == EOF && length == 0)
    {
        free(buffer);
        return NULL;
    }

    // Tolerate CRLF line endings on text streams.
    if (length > 0 && buffer[length - 1] == '\r')
        --length;

    buffer[length] = '\0';
    return buffer;
}
