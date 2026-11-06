/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include "utils/env.h"

bool scli_console_isdumb(void)
{
    return scli_env_tolower_matches("TERM", "dumb") || scli_env_tolower_matches("TERM", "unknown");
}
