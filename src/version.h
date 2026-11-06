/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_VERSION_
#define _H_VERSION_

#include "api.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The version lives in the library rather than in a generated header, so a
 * program always reports the build it is actually linked against.
 */

/* Full version: "0.1.0" when released, "0.1.0+7.gabc123def456[.dirty]" past
 * the tag. */
SCLI_API const char *scli_version_string(void);

/* Release components of the version above. */
SCLI_API unsigned scli_version_major(void);
SCLI_API unsigned scli_version_minor(void);
SCLI_API unsigned scli_version_patch(void);

/* (major << 16) | (minor << 8) | patch, so releases compare with one
 * operator. Build metadata is not part of it. */
SCLI_API uint32_t scli_version_number(void);

/* Commits since the release tag; 0 for a tagged build, and 0 when a source
 * archive does not record a count. */
SCLI_API unsigned scli_version_commits(void);

/* Abbreviated commit the build came from, "" when not known. */
SCLI_API const char *scli_version_commit(void);

/* Whether the work tree had uncommitted changes when it was built. */
SCLI_API bool scli_version_dirty(void);

#ifdef __cplusplus
}
#endif

#endif /* _H_VERSION_ */
