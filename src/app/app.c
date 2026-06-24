#include <stdio.h>

#include "app/app.h"
#include "page/page.h"
#include "page/auto_play_page.h"
#include "page/image_browsing_page.h"
#include "page/image_viewing_page.h"
#include "page/main_menu_page.h"


void app_init(AppState *app)
{
    if(NULL == app)
        return;

    app -> current_page = PAGE_MAIN_MENU;
}

void app_run(AppState *app)
{
    if(NULL == app)
        return;

    while (PAGE_EXIT != app -> current_page)
    {
        printf("\n[debug] current_page: %s\n",
              page_get_name(app -> current_page)
              );

        switch (app -> current_page) 
        {
            case PAGE_MAIN_MENU:
                main_menu_page_run(app);
                break;

            case PAGE_IMAGE_BROWSING:
                image_browsing_page_run(app);
                break;

            case PAGE_IMAGE_VIEWING:
                image_viewing_page_run(app);
                break;

            case PAGE_AUTO_PLAY:
                auto_play_page_run(app);
                break;

            default:
                app->current_page = PAGE_EXIT;
                break;
        }

    }

    printf("LuminaFrame exit.\n");
}
