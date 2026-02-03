/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <sparkcli.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const char *BANNER =
    "  ___                _    ___ _    ___ \n"
    " / __|_ __  __ _ _ _| |__/ __| |  |_ _|\n"
    " \\__ \\ '_ \\/ _` | '_| / / (__| |__ | | \n"
    " |___/ .__/\\__,_|_| |_\\_\\\\___|____|___|\n"
    "     |_|                               \n";

static unsigned char lerp_uc(unsigned char a, unsigned char b, float t)
{
    return (unsigned char)(a + (b - a) * t);
}

/**
 * Generates an N-step swatch from white to black passing by the base color.
 * Caller must free the returned array.
 */
scli_color_t *generate_swatch(scli_color_t base, int steps)
{
    if (steps <= 0)
        return NULL;

    scli_color_t *swatch = malloc(sizeof(scli_color_t) * steps);
    if (!swatch)
        return NULL;

    // Define almost-white and almost-black anchors
    scli_color_t white = {240, 240, 240};
    scli_color_t black = {15, 15, 15};

    for (int i = 0; i < steps; i++)
    {
        float t = (float)i / (float)(steps - 1);

        if (t < 0.5f)
        {
            // First half: white to base
            float u = t * 2.0f; // remap [0, 0.5] → [0, 1]
            swatch[i].r = lerp_uc(white.r, base.r, u);
            swatch[i].g = lerp_uc(white.g, base.g, u);
            swatch[i].b = lerp_uc(white.b, base.b, u);
        }
        else
        {
            // Second half: base to black
            float u = (t - 0.5f) * 2.0f; // remap [0.5, 1] → [0, 1]
            swatch[i].r = lerp_uc(base.r, black.r, u);
            swatch[i].g = lerp_uc(base.g, black.g, u);
            swatch[i].b = lerp_uc(base.b, black.b, u);
        }
    }

    return swatch;
}

int main(void)
{
    printf("%s", BANNER);
    printf("Colors functionality example!\n\n");

    printf("This example demonstrates the color functionality of SparkCLI.\n");
    printf("Colors are only displayed when the terminal supports truecolor.\n");
    printf("If you're seeing this in a terminal that supports colors, you should see colored text below.\n\n");

    printf("Both foreground and background color is supported:\n");
    scli_color_fg((scli_color_t){255, 0, 0});
    printf("- This is red text\n");
    scli_color_fg((scli_color_t){0, 255, 0});
    printf("- This is green text\n");
    scli_color_fg((scli_color_t){0, 0, 255});
    printf("- This is blue text\n");
    scli_color_reset();
    scli_color_bg((scli_color_t){255, 255, 0});
    printf("- Yellow background");
    scli_color_reset();
    printf("\n");
    scli_color_bg((scli_color_t){255, 0, 255});
    printf("- Magenta background");
    scli_color_reset();
    printf("\n");
    scli_color_bg((scli_color_t){0, 255, 255});
    printf(" Cyan background");
    scli_color_reset();
    printf("\n\n");

    printf("Color palette demonstration:\n");
    const int cols = 32;
    scli_color_t base_colors[] = {
        {0xff, 0x00, 0x00},
        {0xff, 0x80, 0x00},
        {0xff, 0xff, 0x00},
        {0x80, 0xff, 0x00},
        {0x00, 0xff, 0x00},
        {0x00, 0xff, 0x80},
        {0x00, 0xff, 0xff},
        {0x00, 0x80, 0xff},
        {0x00, 0x00, 0xff},
        {0x80, 0x00, 0xff},
        {0xff, 0x00, 0xff},
        {0xff, 0x00, 0x80},
        {0x80, 0x80, 0x80},
    };
    for (int row = 0; row < sizeof(base_colors) / sizeof(scli_color_t); row++)
    {
        scli_color_t *swatch = generate_swatch(base_colors[row], cols);
        for (int col = 0; col < cols; col++)
        {
            scli_color_bg(swatch[col]);
            printf("  ");
        }
        free(swatch);
        scli_color_reset();
        printf("\n");
    }

    return 0;
}