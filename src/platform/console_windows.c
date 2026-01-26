/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include <io.h>
#include <stdio.h>

bool console_isatty()
{
    return _isatty(_fileno(stdout));
}
