#include <stdio.h>

#include "page/auto_play_page.h"
#include "page/page.h"

void auto_play_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;

    printf("\n===== Auto Play Page =====\n");
    printf("auto play fake images\n");
    printf("b. Back to main menu\n");
    printf("q. Quit\n");
    printf("input: ");

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
