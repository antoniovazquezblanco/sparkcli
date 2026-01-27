/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_COLOR_
#define _H_COLOR_

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} scli_color_t;

void scli_color_reset();
void scli_color_fg(scli_color_t color);
void scli_color_bg(scli_color_t color);

#endif /* _H_COLOR_ */