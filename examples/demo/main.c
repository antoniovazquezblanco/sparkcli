/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

/*
 * SparkCLI showcase.
 *
 * A single guided tour through every SparkCLI feature:
 * colors, logging, tables, trees, progress bars and hex dumps
 */

#include <sparkcli.h>

#include <stdint.h>
#include <stdio.h>

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

static void beat(int ms)
{
    fflush(stdout);
    wait_ms(ms);
}

static const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";

/* A small palette reused across the demo. */
static const scli_color_t COLOR_TITLE = {0, 200, 255};
static const scli_color_t COLOR_ACCENT = {255, 196, 0};
static const scli_color_t COLOR_MUTED = {130, 130, 150};

/* Print a colored section header. */
static void section(const char *title)
{
    printf("\n");
    scli_color_fg(COLOR_TITLE);
    printf("== %s ==\n", title);
    scli_color_reset();
    beat(450);
}

static void demo_intro(void)
{
    scli_color_fg(COLOR_ACCENT);
    printf("%s", BANNER);
    scli_color_reset();
    beat(700);

    printf("Welcome to the SparkCLI showcase!\n");
    beat(700);

    scli_color_fg(COLOR_MUTED);
    if (scli_color_system() == COLOR_SYSTEM_TRUECOLOR)
        printf("Your terminal supports 24-bit truecolor; enjoy the colors below.\n");
    else
        printf("Your terminal does not report truecolor; output degrades gracefully.\n");
    scli_color_reset();
    beat(600);
}

static void demo_logging(void)
{
    section("Logging");
    scli_log_debug("Booting ground station controller");
    beat(750);
    scli_log_info("Antenna aligned, acquiring downlink");
    beat(750);
    scli_log_warn("Downlink margin low (%d dB)", 3);
    beat(750);
    scli_log_error("Packet %u failed CRC, requesting retransmit", 42u);
}

static void demo_colors(void)
{
    section("Colors");
    printf("Truecolor foreground and background, with graceful fallback:\n");
    beat(400);

    const scli_color_t swatches[] = {
        {239, 71, 111}, {255, 209, 102}, {6, 214, 160}, {17, 138, 178}, {7, 59, 76},
    };
    const char *names[] = {"rouge", "amber", "mint", "azure", "navy"};

    for (size_t i = 0; i < sizeof(swatches) / sizeof(swatches[0]); ++i)
    {
        scli_color_fg(swatches[i]);
        printf("  %-7s", names[i]);
        scli_color_reset();

        scli_color_bg(swatches[i]);
        printf("        ");
        scli_color_reset();
        printf("\n");
        beat(250);
    }
}

static void demo_table(void)
{
    section("Tables");

    char *headers[] = {"Subsystem", "Status", "Telemetry"};
    scli_tbl_t *table = scli_tbl_new(3, headers);
    if (!table)
    {
        scli_log_error("Could not allocate table");
        return;
    }

    char *rows[][3] = {
        {"Power", "Nominal", "28.4 V"},
        {"Thermal", "Nominal", "-12 C"},
        {"Comms", "Degraded", "3 dB"},
        {"Payload", "Standby", "--"},
    };
    for (size_t i = 0; i < sizeof(rows) / sizeof(rows[0]); ++i)
        scli_tbl_add_row(table, rows[i]);

    beat(300);
    scli_tbl_render(table);
    scli_tbl_free(&table);
}

static void demo_tree(void)
{
    section("Trees");

    scli_tree_node_t *root = scli_tree_new("Satellite");
    scli_tree_node_t *aocs = scli_tree_new("AOCS");
    scli_tree_node_t *cdh = scli_tree_new("CD&H");
    if (!root || !aocs || !cdh)
    {
        scli_log_error("Could not allocate tree nodes");
        scli_tree_node_free(root);
        scli_tree_node_free(aocs);
        scli_tree_node_free(cdh);
        return;
    }

    scli_tree_add_child(root, aocs);
    scli_tree_add_child(root, cdh);
    scli_tree_add_child(aocs, scli_tree_new("Reaction wheels"));
    scli_tree_add_child(aocs, scli_tree_new("Star tracker"));
    scli_tree_add_child(cdh, scli_tree_new("OBC"));
    scli_tree_add_child(cdh, scli_tree_new("Memory"));

    beat(300);
    scli_tree_render(root);
    scli_tree_node_free(root);
}

static void demo_progress(void)
{
    section("Progress bars");

    char *labels[] = {"Uploading firmware", "Verifying image"};
    for (size_t t = 0; t < sizeof(labels) / sizeof(labels[0]); ++t)
    {
        const size_t total = 40;
        scli_prg_t *progress = scli_prg_new(total, labels[t]);
        if (!progress)
        {
            scli_log_error("Could not allocate progress bar");
            return;
        }

        for (size_t step = 0; step <= total; ++step)
        {
            scli_prg_update(progress, step);
            wait_ms(30);
        }
        scli_prg_finish(progress);
        scli_prg_free(progress);
        beat(500);
    }
}

static void demo_hexdump(void)
{
    section("Hexdump");
    printf("A 64-byte telemetry frame:\n");
    beat(400);

    uint8_t frame[64];
    const char *tag = "SparkCLI telemetry frame v1";
    size_t i = 0;
    for (; i < sizeof(frame) && tag[i] != '\0'; ++i)
        frame[i] = (uint8_t)tag[i];
    for (; i < sizeof(frame); ++i)
        frame[i] = (uint8_t)(i * 7);

    scli_hexdump16(frame, sizeof(frame));
}

int main(void)
{
    demo_intro();
    beat(900);
    demo_logging();
    beat(900);
    demo_colors();
    beat(900);
    demo_table();
    beat(900);
    demo_tree();
    beat(900);
    demo_progress();
    beat(900);
    demo_hexdump();
    beat(900);

    printf("\n");
    scli_color_fg(COLOR_ACCENT);
    printf("That's SparkCLI -- happy hacking!\n");
    scli_color_reset();
    return 0;
}
