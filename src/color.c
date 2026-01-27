/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "color.h"

#include "platform/console.h"

#include <stdio.h>

typedef enum
{
    COLOR_SYSTEM_NONE = 0, // No color support
    COLOR_SYSTEM_TRUECOLOR,
} _color_system_t;

static _color_system_t _get_color_system(void)
{
    // Check that the current console is not a dumb terminal...
    if (!console_isatty() || console_isdumb())
        return COLOR_SYSTEM_NONE;

    if (console_hastruecolor())
        return COLOR_SYSTEM_TRUECOLOR;

    // Unknown color system, default to none...
    return COLOR_SYSTEM_NONE;
}

void _truecolor_reset()
{
    printf("\x1b[0m");
}

void _truecolor_fg(scli_color_t color)
{
    printf("\x1b[38;2;%d;%d;%dm", color.r, color.g, color.b);
}

void _truecolor_bg(scli_color_t color)
{
    printf("\x1b[48;2;%d;%d;%dm", color.r, color.g, color.b);
}

void scli_color_reset()
{
    switch (_get_color_system())
    {
    case COLOR_SYSTEM_NONE:
        return;
    case COLOR_SYSTEM_TRUECOLOR:
        _truecolor_reset();
        break;
    }
}

void scli_color_fg(scli_color_t color)
{
    switch (_get_color_system())
    {
    case COLOR_SYSTEM_NONE:
        return;
    case COLOR_SYSTEM_TRUECOLOR:
        _truecolor_fg(color);
        break;
    }
}

void scli_color_bg(scli_color_t color)
{
    switch (_get_color_system())
    {
    case COLOR_SYSTEM_NONE:
        return;
    case COLOR_SYSTEM_TRUECOLOR:
        _truecolor_bg(color);
        break;
    }
}
