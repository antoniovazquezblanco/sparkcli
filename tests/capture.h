/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#ifndef _H_TEST_CAPTURE_
#define _H_TEST_CAPTURE_

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <process.h>
#define capture_dup _dup
#define capture_dup2 _dup2
#define capture_close _close
#define capture_fileno _fileno
#define capture_getpid _getpid
#else
#include <unistd.h>
#define capture_dup dup
#define capture_dup2 dup2
#define capture_close close
#define capture_fileno fileno
#define capture_getpid getpid
#endif

static int _capture_saved_fd = -1;
static FILE *_capture_file = NULL;
static char _capture_path[64];

/* Test binaries run in parallel from a shared working directory, so the
 * capture file has to be unique to this process. */
static inline const char *_capture_path_for_this_process(void)
{
    if (_capture_path[0] == '\0')
        snprintf(_capture_path, sizeof(_capture_path), "capture_%ld.tmp", (long)capture_getpid());
    return _capture_path;
}

/* Point the standard output file descriptor at a file. The stdout stream
 * itself is left alone, which freopen() would otherwise re-associate. */
static inline void capture_start(void)
{
    fflush(stdout);
    _capture_saved_fd = capture_dup(capture_fileno(stdout));
    _capture_file = fopen(_capture_path_for_this_process(), "wb");
    if (_capture_file)
        capture_dup2(capture_fileno(_capture_file), capture_fileno(stdout));
}

/* Restore the standard output and return what was written, NUL terminated. */
static inline size_t capture_stop(char *buffer, size_t buffer_size)
{
    fflush(stdout);

    if (_capture_saved_fd != -1)
    {
        capture_dup2(_capture_saved_fd, capture_fileno(stdout));
        capture_close(_capture_saved_fd);
        _capture_saved_fd = -1;
    }
    if (_capture_file)
    {
        fclose(_capture_file);
        _capture_file = NULL;
    }

    buffer[0] = '\0';
    FILE *f = fopen(_capture_path_for_this_process(), "rb");
    if (!f)
        return 0;
    size_t read_bytes = fread(buffer, 1, buffer_size - 1, f);
    buffer[read_bytes] = '\0';
    fclose(f);
    remove(_capture_path_for_this_process());

    /* stdout stays in text mode on Windows, so drop the carriage returns it
     * adds and let assertions be written against plain newlines. */
    size_t kept = 0;
    for (size_t i = 0; i < read_bytes; ++i)
    {
        if (buffer[i] == '\r' && i + 1 < read_bytes && buffer[i + 1] == '\n')
            continue;
        buffer[kept++] = buffer[i];
    }
    buffer[kept] = '\0';
    return kept;
}

#endif /* _H_TEST_CAPTURE_ */
