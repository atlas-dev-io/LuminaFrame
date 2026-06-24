#include <stdio.h>

#include "page/auto_play_page.h"
#include "display/display.h"
#include "page/page.h"

void auto_play_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;
    display_clear();
    display_show_line(0,"===== Auto Play Page =====");
    display_show_line(1,"auto play fake images");
    display_show_line(2,"b. Back to main menu");
    display_show_line(3,"q. Quit");
    display_show_line(4,"input: ");
    display_refresh();

    ch = getchar();

    while (getchar() != '\n')
        ;

    switch (ch) {
    case 'b':
        app->current_page = PAGE_MAIN_MENU;
        break;

    case 'q':
        app->current_page = PAGE_EXIT;
        break;

    default:
        app->current_page = PAGE_AUTO_PLAY;
        break;
    }
}
