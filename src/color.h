/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_COLOR_
#define _H_COLOR_

typedef enum
{
    COLOR_SYSTEM_NONE = 0,
    COLOR_SYSTEM_TRUECOLOR,
} scli_color_system_t;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} scli_color_t;

scli_color_system_t scli_color_system();

void scli_color_reset();
void scli_color_fg(scli_color_t color);
void scli_color_bg(scli_color_t color);

#endif /* _H_COLOR_ */
