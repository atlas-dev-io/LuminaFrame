#include <stdio.h>

#include "page/auto_play_page.h"
#include "display/display.h"
#include "page/page.h"
#include "input/input.h"

void auto_play_page_run(AppState *app)
{
    InputEvent event;

    if (app == NULL)
        return;
    display_clear();
    display_show_line(0,"===== Auto Play Page =====");
    display_show_line(1,"auto play fake images");
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
        app -> current_page = PAGE_AUTO_PLAY;
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
        app->current_page = PAGE_AUTO_PLAY;
        break;
    }
}
