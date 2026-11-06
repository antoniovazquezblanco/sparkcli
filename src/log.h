/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_LOG_
#define _H_LOG_

#include "api.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__MINGW32__)
#define SCLI_PRINTF(fmt_index, args_index) __attribute__((format(gnu_printf, fmt_index, args_index)))
#elif defined(__GNUC__) || defined(__clang__)
#define SCLI_PRINTF(fmt_index, args_index) __attribute__((format(printf, fmt_index, args_index)))
#else
#define SCLI_PRINTF(fmt_index, args_index)
#endif

/* Log levels, ordered by increasing severity. */
typedef enum
{
    SCLI_LOGLEVEL_DEBUG = 0,
    SCLI_LOGLEVEL_INFO,
    SCLI_LOGLEVEL_WARN,
    SCLI_LOGLEVEL_ERROR,
    SCLI_LOGLEVEL_NONE
} scli_loglevel_t;

/**
 * Set the minimum level a message must reach to be printed.
 *
 * Messages below @level are discarded. Pass SCLI_LOGLEVEL_NONE to silence the
 * logger entirely. The default is SCLI_LOGLEVEL_DEBUG, and the SCLI_LOG_LEVEL
 * environment variable overrides it until the first call to this function.
 */
void scli_log_level_set(scli_loglevel_t level);

/**
 * Get the minimum level a message must reach to be printed.
 */
scli_loglevel_t scli_log_level_get(void);

/* Logging functions */
SCLI_API void scli_log_debug(const char *format, ...) SCLI_PRINTF(1, 2);
SCLI_API void scli_log_info(const char *format, ...) SCLI_PRINTF(1, 2);
SCLI_API void scli_log_warn(const char *format, ...) SCLI_PRINTF(1, 2);
SCLI_API void scli_log_error(const char *format, ...) SCLI_PRINTF(1, 2);

#ifdef __cplusplus
}
#endif

#endif /* _H_LOG_ */
