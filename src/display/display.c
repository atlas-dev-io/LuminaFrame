#include <stdio.h>

#include "display/display.h"


#define DISPLAY_SIMULATED_WIDTH  800
#define DISPLAY_SIMULATED_HEIGHT 480

int display_init(void)
{
    return 0; 
}


int display_get_width(void)
{
    /*
     * Terminal display backend has no real pixel size.
     * Return a simulated LCD width for image resize testing.
     */
    return DISPLAY_SIMULATED_WIDTH;
}

int display_get_height(void)
{
    /*
     * Terminal display backend has no real pixel size.
     * Return a simulated LCD height for image resize testing.
     */
    return DISPLAY_SIMULATED_HEIGHT;
}

void display_clear(void)
{
/*
     * ANSI escape sequence:
     * \033[2J clears the screen.
     * \033[H moves cursor to top-left.
     */
    printf("\033[2J");
    printf("\033[H");
}

void display_show_line(int row, const char *text)
{
    /*
     * ANSI escape sequence:
     * \033[%d;1H moves cursor to row, column 1.
     *
     * Terminal row starts from 1, so row + 1 is used.
     */
    printf("\033[%d;1H", row + 1);
    printf("%s", text);
}


void display_refresh(void)
{
    fflush(stdout);
}

void display_close(void)
{
    printf("\033[0m");
    fflush(stdout);
}

