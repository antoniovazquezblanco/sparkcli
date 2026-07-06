/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include "utils/env.h"

#include <unistd.h>

bool console_stdout_isatty()
{
    return isatty(STDOUT_FILENO);
}

bool console_stdin_isatty()
{
    return isatty(STDIN_FILENO);
}

bool console_hastruecolor()
{
    return env_tolower_matches("COLORTERM", "truecolor") || env_tolower_matches("COLORTERM", "24bit");
}

bool console_set_utf8_output(void)
{
    return true;
}
