/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "env.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

bool env_tolower_matches(char *varname, char *value)
{
    // Get the term variable
    const char *varval = getenv(varname);
    if (!varval)
        return false;

    // We may not modify data returned by getenv so copy it and convert to lower case
    char *varval_lower = strdup(varval);
    if (!varval_lower)
        return false;
    for (int i = 0; varval_lower[i]; i++)
        varval_lower[i] = tolower(varval_lower[i]);

    // Test if the term is dumb
    bool match = (strcmp(varval_lower, value) == 0);

    // Cleanup...
    free(varval_lower);
    return match;
}
