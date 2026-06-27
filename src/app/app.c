#include <stdio.h>

#include "app/app.h"
#include "page/page.h"
#include "page/auto_play_page.h"
#include "page/image_browsing_page.h"
#include "page/image_viewing_page.h"
#include "page/main_menu_page.h"
#include "display/display.h"
#include "input/input.h"

void app_init(AppState *app)
{
    if(NULL == app)
        return;

    app -> current_page          = PAGE_MAIN_MENU;
    app -> current_file_path[0]  = '\0'          ;

    browser_init(&app -> browser);

    if (display_init() < 0) {
        app -> current_page = PAGE_EXIT;
        return ;
    }

    if(input_init() < 0){
        display_close();
        app -> current_page = PAGE_EXIT;
        return ;
    }
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
                app-> current_page = PAGE_EXIT;
                break;
        }

    }

    input_close();
    display_close();
    printf("LuminaFrame exit.\n");
}

int app_set_current_file_from_browser(AppState *app)
{
    const char *dir_path;
    const char *file_name;
    int ret;

    if(NULL == app)
        return -1;

    dir_path  = browser_get_current_dir(&app -> browser);
    file_name = browser_get_selected_entry(&app -> browser);

    if(NULL == dir_path || NULL == file_name){
        app -> current_file_path[0] = '\0';
        return -1;
    }

    if('\0' == dir_path[0]){
        app -> current_file_path[0] = '\0';
        return -1;
    }

    ret = snprintf(app -> current_file_path,
                   sizeof(app -> current_file_path),
                   "%s/%s",
                   dir_path,
                   file_name);

    if(ret < 0 || (size_t)ret >= sizeof(app -> current_file_path)){
        app -> current_file_path[0] = '\0';
        return -1;
    }

    return 0;
}
