/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_STR_
#define _H_STR_

#include <stdlib.h>
#include <string.h>

/* Evaluates its argument twice: do not pass an expression with side effects. */
#define str_safe(val) ((val) ? (val) : "")

/* Returns a buffer the caller must free(), or NULL on allocation failure. */
static inline char *str_dup(const char *str)
{
    if (!str)
        return NULL;
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (!copy)
        return NULL;
    memcpy(copy, str, len);
    return copy;
}

#endif /* _H_STR_ */
