#include <stdio.h>

#include "display/display.h"


int display_init(void)
{
    return 0; 
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

