/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include "utils/env.h"

#include <unistd.h>

bool console_isatty()
{
    return isatty(STDOUT_FILENO);
}

bool console_hastruecolor()
{
    return env_tolower_matches("COLORTERM", "truecolor") || env_tolower_matches("COLORTERM", "24bit");
}
