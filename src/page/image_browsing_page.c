#include <stdio.h>

#include "page/image_browsing_page.h"
#include "display/display.h"
#include "page/page.h"

void image_browsing_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;
    display_clear();
    display_show_line(0,"===== Image Browsing Page =====");
    display_show_line(1,"fake image 1: test1.jpg");
    display_show_line(2,"fake image 2: test2.jpg");
    display_show_line(3,"v. View image");
    display_show_line(4,"b. Back to main menu");
    display_show_line(5,"q. Quit");
    display_show_line(6,"input: ");
    display_refresh();

    ch = getchar();

    while (getchar() != '\n')
        ;

    switch (ch) {
    case 'v':
        app->current_page = PAGE_IMAGE_VIEWING;
        break;

    case 'b':
        app->current_page = PAGE_MAIN_MENU;
        break;

    case 'q':
        app->current_page = PAGE_EXIT;
        break;

    default:
        app->current_page = PAGE_IMAGE_BROWSING;
        break;
    }
}
