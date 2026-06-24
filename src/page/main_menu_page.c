#include <stdio.h>

#include "page/page.h"
#include "page/main_menu_page.h"
#include "display/display.h"

void main_menu_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;

    display_show_line(0,"===== Main Menu =====");
    display_show_line(1,"1. Image browsing");
    display_show_line(2,"2. Image viewing");
    display_show_line(3,"3. Auto play");
    display_show_line(4,"q. Quit");
    display_show_line(5,"input: ");
    display_refresh();

    ch = getchar();

    while (getchar() != '\n')
        ;

    switch (ch) {
    case '1':
        app->current_page = PAGE_IMAGE_BROWSING;
        break;

    case '2':
        app->current_page = PAGE_IMAGE_VIEWING;
        break;

    case '3':
        app->current_page = PAGE_AUTO_PLAY;
        break;

    case 'q':
        app->current_page = PAGE_EXIT;
        break;

    default:
        app->current_page = PAGE_MAIN_MENU;
        break;
    }
}
