/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include "utils/env.h"

#include <stdio.h>
#include <termios.h>
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

int console_getkey(void)
{
    // Fall back to a plain read when stdin is not a terminal (e.g. a pipe).
    struct termios original;
    if (tcgetattr(STDIN_FILENO, &original) != 0)
        return getchar();

    // Switch to raw mode so a single keypress is returned without echo.
    struct termios raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    return c;
}

bool console_set_utf8_output(void)
{
    return true;
}
