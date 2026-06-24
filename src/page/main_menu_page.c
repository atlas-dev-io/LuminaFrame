#include <stdio.h>

#include "page/main_menu_page.h"
#include "page/page.h"

void main_menu_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;

    printf("\n===== Main Menu =====\n");
    printf("1. Image browsing\n");
    printf("2. Image viewing\n");
    printf("3. Auto play\n");
    printf("q. Quit\n");
    printf("input: ");

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
