#include <stdio.h>

#include "page/image_viewing_page.h"
#include "display/display.h"
#include "page/page.h"

void image_viewing_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;
    display_clear();
    display_show_line(0,"===== Image Viewing Page =====");
    display_show_line(1,"show fake image here");
    display_show_line(2,"b. Back to image browsing");
    display_show_line(3,"q. Quit");
    display_show_line(4,"input: ");
    display_refresh();

    ch = getchar();

    while (getchar() != '\n')
        ;

    switch (ch) {
    case 'b':
        app->current_page = PAGE_IMAGE_BROWSING;
        break;

    case 'q':
        app->current_page = PAGE_EXIT;
        break;

    default:
        app->current_page = PAGE_IMAGE_VIEWING;
        break;
    }
}
