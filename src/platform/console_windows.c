/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include "console.h"

#include <io.h>
#include <stdio.h>
#include <windows.h>

bool console_isatty()
{
    return _isatty(_fileno(stdout));
}

static bool _is_virtual_terminal()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return false;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return false;

    return (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}

bool console_hastruecolor()
{
    // If not in a VT, no truecolor...
    if (!_is_virtual_terminal())
        return false;

    // If VT, some windows versions, truecolor is available...
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (!GetVersionEx((OSVERSIONINFO *)&osvi))
        // Windows version not available...
        return false;

    // Set truecolor flag on Windows 10+...
    return (osvi.dwMajorVersion > 10) || (osvi.dwMajorVersion == 10 && osvi.dwBuildNumber >= 15063);
}

bool console_set_utf8_output(void)
{
    return SetConsoleOutputCP(CP_UTF8) != 0;
}
