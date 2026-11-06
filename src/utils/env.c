/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "env.h"

#include "str.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool scli_env_tolower_matches(char *varname, char *value)
{
    // Get the term variable
    const char *varval = getenv(varname);
    if (!varval)
        return false;

    // We may not modify data returned by getenv so copy it and convert to lower case
    char *varval_lower = str_dup(varval);
    if (!varval_lower)
        return false;
    // tolower() is only defined for values representable as unsigned char or
    // EOF, so a byte over 0x7f must not reach it as a negative plain char.
    for (size_t i = 0; varval_lower[i]; i++)
        varval_lower[i] = (char)tolower((unsigned char)varval_lower[i]);

    // Test if the term is dumb
    bool match = (strcmp(varval_lower, value) == 0);

    // Cleanup...
    free(varval_lower);
    return match;
}

bool scli_env_is_set(const char *varname)
{
    const char *varval = getenv(varname);
    return varval != NULL && varval[0] != '\0';
}
