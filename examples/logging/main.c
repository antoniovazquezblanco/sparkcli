/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * 
 * Author: Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 */

#include <sparkcli.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* Cross-platform sleep function */
static void sleep_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

int main(void)
{    
    /* Log at different levels */
    scli_log_debug("This is a debug message");
    scli_log_info("This is an info message");
    scli_log_warn("This is a warning message");
    scli_log_error("This is an error message");
    sleep_ms(2000);
    
    /* Demonstrate timestamp behavior with rapid messages */
    scli_log_info("First message");
    scli_log_info("Second message (same second)");
    scli_log_info("Third message (same second)");
    scli_log_info("Error message (same second)");
    sleep_ms(2000);
    scli_log_info("Message after 2 seconds");
    scli_log_info("Another warning after delay (same second)");
    sleep_ms(2000);
    scli_log_info("Final message");
    scli_log_info("Another final message (same second)");
    
    return 0;
}