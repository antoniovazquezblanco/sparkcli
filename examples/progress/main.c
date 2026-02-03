/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <sparkcli.h>
#include <stdio.h>

const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";

#ifdef _WIN32
#include <windows.h>
static void wait_ms(int ms)
{
    Sleep(ms);
}
#else
#include <time.h>
static void wait_ms(int ms)
{
    struct timespec delay = {.tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000L};
    nanosleep(&delay, NULL);
}
#endif

int main(void)
{
    printf("%s", BANNER);
    printf("Progress bars example for SparkCLI\n\n");

    const size_t total = 60;
    scli_prg_t *progress = scli_prg_new(total, "Rendering");
    if (!progress)
        return 1;

    for (size_t current = 0; current <= total; ++current)
    {
        scli_prg_update(progress, current);
        wait_ms(50);
    }
    scli_prg_finish(progress);
    scli_prg_free(progress);

    puts("Done!");
    return 0;
}
