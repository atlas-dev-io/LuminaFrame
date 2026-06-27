#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "app/app.h"
#include "page/image_browsing_page.h"
#include "display/display.h"
#include "page/page.h"
#include "input/input.h"
#include "browser/browser.h"


#define IMAGE_BROWSING_MAX_SHOW 8

void image_browsing_page_run(AppState *app)
{
    InputEvent      event       ;
    size_t          count       ;
    size_t          i           ;
    size_t          index       ;
    size_t          show_count  ;
    char            line[1024]  ;
    const char      *file_name  ;
    const char      *dir_path   ;

    if (app == NULL)
        return;

    dir_path = browser_get_current_dir(&app -> browser);
    if(NULL == dir_path)
        dir_path = "";

    display_clear();
    display_show_line(0,"===== Image Browsing Page =====");

    if(browser_load_dir(&app -> browser, ".") < 0){
        display_show_line(1, "Failed to load current directory.");
        display_show_line(2,"b. Back to main menu");
        display_show_line(3,"q. Quit");
        display_show_line(4,"input: ");
        display_refresh();

        event = input_read_event();

        if (event.type == INPUT_EVENT_QUIT) {
            app -> current_page = PAGE_EXIT;
            return;
        }

        if (event.type != INPUT_EVENT_KEY) {
            app -> current_page = PAGE_IMAGE_BROWSING;
            return;
        }

        switch (event.key) {
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

        return ;
    }

    count       = browser_get_count(&app -> browser)      ;
    show_count  = count                                            ;

    if(show_count > IMAGE_BROWSING_MAX_SHOW)
        show_count = IMAGE_BROWSING_MAX_SHOW;

    snprintf(line, 
             sizeof(line), 
             "Directory: %s", 
             dir_path);
    display_show_line(1, line);

    snprintf(line, 
             sizeof(line), 
             "Files: %zu", 
             count);
    display_show_line(2, line);

    if(0 == show_count)
        display_show_line(3, "No image files found.");
    else{
        for (i = 0; i < show_count; i++) {
            file_name = browser_get_entry(&app -> browser, i);
            if(NULL == file_name)
                continue;

            snprintf(line, 
                    sizeof(line), 
                    "%zu. %s", 
                    i + 1,
                    file_name);

            display_show_line((int)i + 3, line);
        }
    }

    display_show_line(12, "b. Back to main menu");
    display_show_line(13, "q. Quit");
    display_show_line(14, "input number to view: ");
    display_refresh();

    event = input_read_event();

    if (event.type == INPUT_EVENT_QUIT) {
        app->current_page = PAGE_EXIT;
        return;
    }

    if (event.type != INPUT_EVENT_KEY) {
        app->current_page = PAGE_IMAGE_BROWSING;
        return;
    }

    switch (event.key) {
        case 'b':
            app -> current_page = PAGE_MAIN_MENU;
            break;
    
        case 'q':
            app -> current_page = PAGE_EXIT;
            break;

        default:
            if(event.key >= '1' && event.key <= '8'){
                index = (size_t)(event.key -'1');

                if(index < show_count){
                    if(0 == browser_select_entry(&app -> browser, 
                                                 index))
                    {
                        if(0 == app_set_current_file_from_browser(app)){
                            app -> current_page = PAGE_IMAGE_VIEWING;
                            break;
                        }
                    }
                }
            }

            app -> current_page = PAGE_IMAGE_BROWSING;
            break;
    }
}
