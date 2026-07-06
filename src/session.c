/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "session.h"

#include "color.h"
#include "platform/console.h"

bool scli_ses_interactive(void)
{
    // Interactive means a user is on both ends: input comes from a terminal
    // and output goes to a terminal that is able to render our escapes.
    return console_stdout_isatty() && console_stdin_isatty() && !console_isdumb();
}

bool scli_ses_color(void)
{
    return scli_color_system() != COLOR_SYSTEM_NONE;
}

bool scli_ses_truecolor(void)
{
    return scli_color_system() == COLOR_SYSTEM_TRUECOLOR;
}
