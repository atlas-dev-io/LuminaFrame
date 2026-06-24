#include <stdio.h>

#include "page/image_browsing_page.h"
#include "page/page.h"

void image_browsing_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;

    printf("\n===== Image Browsing Page =====\n");
    printf("fake image 1: test1.jpg\n");
    printf("fake image 2: test2.jpg\n");
    printf("v. View image\n");
    printf("b. Back to main menu\n");
    printf("q. Quit\n");
    printf("input: ");

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
