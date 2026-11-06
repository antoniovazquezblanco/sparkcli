/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_ENV_
#define _H_ENV_

#include <stdbool.h>

bool scli_env_tolower_matches(char *varname, char *value);

/**
 * Test whether @varname is present in the environment and not empty.
 */
bool scli_env_is_set(const char *varname);

#endif /* _H_ENV_ */
