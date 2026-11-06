/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "log.h"

#include "color.h"
#include "utils/env.h"

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

static const scli_color_t _loglevel_fg_colors[SCLI_LOGLEVEL_NONE] = {
    [SCLI_LOGLEVEL_DEBUG] = {120, 130, 110},
    [SCLI_LOGLEVEL_INFO] = {110, 179, 117},
    [SCLI_LOGLEVEL_WARN] = {214, 128, 39},
    [SCLI_LOGLEVEL_ERROR] = {220, 50, 47},
};

static const char *const _loglevel_strings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "NONE",
};

/* Track last log timestamp to avoid duplicate timestamps */
static time_t g_last_log_time = 0;

/* Messages below this level are discarded. */
static scli_loglevel_t g_log_level = SCLI_LOGLEVEL_DEBUG;
static bool g_log_level_initialized = false;

/**
 * Read the threshold from SCLI_LOG_LEVEL, leaving it untouched when the
 * variable is absent or holds something we do not recognise.
 */
static void _log_level_init_from_env(void)
{
    g_log_level_initialized = true;
    if (scli_env_tolower_matches("SCLI_LOG_LEVEL", "debug"))
        g_log_level = SCLI_LOGLEVEL_DEBUG;
    else if (scli_env_tolower_matches("SCLI_LOG_LEVEL", "info"))
        g_log_level = SCLI_LOGLEVEL_INFO;
    else if (scli_env_tolower_matches("SCLI_LOG_LEVEL", "warn"))
        g_log_level = SCLI_LOGLEVEL_WARN;
    else if (scli_env_tolower_matches("SCLI_LOG_LEVEL", "error"))
        g_log_level = SCLI_LOGLEVEL_ERROR;
    else if (scli_env_tolower_matches("SCLI_LOG_LEVEL", "none"))
        g_log_level = SCLI_LOGLEVEL_NONE;
}

void scli_log_level_set(scli_loglevel_t level)
{
    g_log_level_initialized = true;
    g_log_level = level;
}

scli_loglevel_t scli_log_level_get(void)
{
    if (!g_log_level_initialized)
        _log_level_init_from_env();
    return g_log_level;
}

/* Get current timestamp string */
static void _timestamp(char *buffer, size_t buffer_size, time_t now)
{
    struct tm *tm_info = localtime(&now);
    strftime(buffer, buffer_size, "%H:%M:%S", tm_info);
}

/* Core logging function */
static void _log_print(scli_loglevel_t level, const char *format, va_list args)
{
    if (level < scli_log_level_get())
        return;

    time_t now = time(NULL);
    bool show_timestamp = (now != g_last_log_time);
    g_last_log_time = now;

    /* Print timestamp if needed */
    if (show_timestamp)
    {
        char timestamp[9];
        _timestamp(timestamp, sizeof(timestamp), now);
        printf("[%s] ", timestamp);
    }
    else
    {
        printf("           ");
    }

    /* Print level */
    scli_color_fg(_loglevel_fg_colors[level]);
    printf("%-5s", _loglevel_strings[level]);
    scli_color_reset();
    printf(" ");

    /* Print message */
    vprintf(format, args);
    printf("\n");
    fflush(stdout);
}

void scli_log_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_DEBUG, format, args);
    va_end(args);
}
void scli_log_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_INFO, format, args);
    va_end(args);
}
void scli_log_warn(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_WARN, format, args);
    va_end(args);
}
void scli_log_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _log_print(SCLI_LOGLEVEL_ERROR, format, args);
    va_end(args);
}
