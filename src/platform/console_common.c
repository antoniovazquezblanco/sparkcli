/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include "utils/env.h"

bool console_isdumb()
{
    return env_tolower_matches("TERM", "dumb") || env_tolower_matches("TERM", "unknown");
}
