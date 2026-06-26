#include <stdio.h>

#include "app/app.h"
#include "page/image_viewing_page.h"
#include "display/display.h"
#include "page/page.h"
#include "input/input.h"

void image_viewing_page_run(AppState *app)
{
    InputEvent event;

    if (NULL == app)
        return;


    display_clear();
    display_show_line(0,"===== Image Viewing Page =====");
    display_show_line(1,"Current file:");

    if('\0'== app -> current_file_path[0])
        display_show_line(2, "No file selected.");
    else
        display_show_line(2, app -> current_file_path);

    display_show_line(4,"b. Back to image browsing");
    display_show_line(5,"q. Quit");
    display_show_line(6,"input: ");
    display_refresh();

    event = input_read_event();

    if (event.type == INPUT_EVENT_QUIT) {
        app -> current_page = PAGE_EXIT;
        return;
    }

    if (event.type != INPUT_EVENT_KEY) {
        app -> current_page = PAGE_IMAGE_VIEWING;
        return;
    }


    switch (event.key) {
        case 'b':
            app-> current_page = PAGE_IMAGE_BROWSING;
            break;

        case 'q':
            app-> current_page = PAGE_EXIT;
            break;

        default:
            app-> current_page = PAGE_IMAGE_VIEWING;
            break;
    }
}
