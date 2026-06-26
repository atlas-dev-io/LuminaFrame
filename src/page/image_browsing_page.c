#include <stddef.h>
#include <stdio.h>

#include "page/image_browsing_page.h"
#include "display/display.h"
#include "page/page.h"
#include "input/input.h"
#include "browser/browser.h"


#define PAGE_MAX_LIST_ITEMS 8


void image_browsing_page_run(AppState *app)
{
    BrowserState    browser     ;
    InputEvent      event       ;
    size_t          count       ;
    size_t          i           ;
    size_t          show_count  ;
    char            line[1024]  ;

    if (app == NULL)
        return;

    display_clear();
    display_show_line(0,"===== Image Browsing Page =====");

    if(browser_load_dir(&browser, ".") < 0){
        display_show_line(1, "Failed to load current directory.");
        display_show_line(2,"b. Back to main menu");
        display_show_line(3,"q. Quit");
        display_show_line(4,"input: ");
        display_refresh();
    }else{
        count       = browser_get_count(&browser);
        show_count  = count                               ;

        if(show_count > PAGE_MAX_LIST_ITEMS)
            show_count = PAGE_MAX_LIST_ITEMS;

        snprintf(line,
         sizeof(line),
         "Directory: %s",
                 browser.current_dir);
        display_show_line(1, line);

        snprintf(line, 
                 sizeof(line), 
                 "Files: %zu", 
                 count);
        display_show_line(2, line);

        if(0 == count)
            display_show_line(3, "No ordinary files found.");
        else{
            for(i = 0; i < show_count; i++){
                snprintf(line,
                         sizeof(line), 
                         "%zu. %s", 
                         i + 1,
                         browser_get_entry(&browser, i));

                display_show_line((int)i + 3, line);
            }
        }

        display_show_line(12, "b. Back to main menu");
        display_show_line(13, "q. Quit");
        display_show_line(14, "input: ");
        display_refresh();
    }

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
}
