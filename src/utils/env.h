/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_ENV_
#define _H_ENV_

#include <stdbool.h>

bool env_tolower_matches(char *varname, char *value);

#endif /* _H_ENV_ */