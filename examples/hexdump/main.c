/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Kevin Leon
 */

#ifndef _WIN32
/* Make nanosleep() visible regardless of the c_std in effect. */
#define _POSIX_C_SOURCE 200809L
#endif

#include <sparkcli.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";

/* Cross-platform sleep function */
static void sleep_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    struct timespec delay = {.tv_sec = milliseconds / 1000, .tv_nsec = (milliseconds % 1000) * 1000000L};
    nanosleep(&delay, NULL);
#endif
}

uint8_t buffer[] = {
    0x53, 0x70, 0x61, 0x72, 0x6b, 0x63, 0x6c, 0x69, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33,
    0x53, 0x70, 0x61, 0x72, 0x6b, 0x63, 0x6c, 0x69, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33,
    0x53, 0x70, 0x61, 0x72, 0x6b, 0x63, 0x6c, 0x69, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33,
    0x53, 0x70, 0x61, 0x72, 0x6b, 0x63, 0x6c, 0x69, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33};

int main(void)
{
    printf("%s", BANNER);
    printf("Tree helper example for SparkCLI\n\n");
    sleep_ms(1000);

    printf("Example Hexdump output 8 block:\n");
    scli_hexdump8(buffer, sizeof(buffer));
    printf("\n");
    sleep_ms(500);

    printf("Example Hexdump output 16 block:\n");
    scli_hexdump16(buffer, sizeof(buffer));
    printf("\n");
    sleep_ms(500);

    printf("Example Hexdump output 32 block:\n");
    scli_hexdump32(buffer, sizeof(buffer));
    printf("\n");
    sleep_ms(1000);

    printf("Example Hexdump output 24 custom block len:\n");
    scli_hexdump(buffer, sizeof(buffer), 24);
    printf("\n\n");
    sleep_ms(1000);

    printf("Example Hex output 8 block:\n");
    scli_hex8(buffer, sizeof(buffer));
    printf("\n");
    sleep_ms(1000);

    printf("Example Hex output 16 block:\n");
    scli_hex16(buffer, sizeof(buffer));
    printf("\n");
    sleep_ms(1000);

    printf("Example Hex output 32 block:\n");
    scli_hex32(buffer, sizeof(buffer));
    printf("\n");
    sleep_ms(1000);

    printf("Example Hex output 24 custom block len:\n");
    scli_hex(buffer, sizeof(buffer), 24);
    return 0;
}