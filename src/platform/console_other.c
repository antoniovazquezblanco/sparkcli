/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include <unistd.h>

bool console_isatty()
{
    return isatty(STDOUT_FILENO);
}
