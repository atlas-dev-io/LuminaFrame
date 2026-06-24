#include <stdio.h>

#include "page/image_viewing_page.h"
#include "page/page.h"

void image_viewing_page_run(AppState *app)
{
    int ch;

    if (app == NULL)
        return;

    printf("\n===== Image Viewing Page =====\n");
    printf("show fake image here\n");
    printf("b. Back to image browsing\n");
    printf("q. Quit\n");
    printf("input: ");

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
