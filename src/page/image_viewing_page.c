#include <stdio.h>


#include "app/app.h"
#include "page/image_viewing_page.h"
#include "browser/browser.h"
#include "display/display.h"
#include "image/image.h"
#include "page/page.h"
#include "input/input.h"


/*
 * Simulated screen area.
 *
 * This step only verifies the memory chain:
 * image_load() -> image_resize_to_fit() -> show resized information.
 *
 * Framebuffer drawing will be added later.
 */
#define IMAGE_VIEWING_FIT_WIDTH   800
#define IMAGE_VIEWING_FIT_HEIGHT  480

void image_viewing_page_run(AppState *app)
{
    InputEvent    event       ;
    Image         image       ;
    Image         resize      ;
    char          line[1024]  ;

    if (NULL == app)
        return;

    image_init(&image);
    image_init(&resize);

    display_clear();
    display_show_line(0,"===== Image Viewing Page =====");
    display_show_line(1,"Current file:");

    if('\0'== app -> current_file_path[0]){
        display_show_line(2, "No file selected.");
        display_show_line(3, "Decode: Failed");
    }else{
        display_show_line(2, app -> current_file_path);

        if(0 == image_load(app -> current_file_path, 
                           &image))
        {
            snprintf(line, 
                     sizeof(line), 
                     "Image: %dx%d, %dbpp",
                     image.width,
                     image.height,
                     image.bpp);

            display_show_line(3, line);

            snprintf(line, 
                     sizeof(line), 
                     "Fit area: %dx%d",
                     IMAGE_VIEWING_FIT_WIDTH,
                     IMAGE_VIEWING_FIT_HEIGHT);

            display_show_line(4, line);

          if(0 == image_resize_to_fit(&image, 
                                      IMAGE_VIEWING_FIT_WIDTH, 
                                      IMAGE_VIEWING_FIT_HEIGHT, 
                                      &resize))
          {
              snprintf(line, 
                       sizeof(line), 
                       "Scaled: %dx%d, %dbpp",
                       resize.width,
                       resize.height,
                       resize.bpp);
              
              display_show_line(5, line);
              display_show_line(6, "Resize: OK");
          }else{
              display_show_line(5, "Scaled: Failed");
              display_show_line(6, "Resize: Failed");
          }

          display_show_line(7, "Decode: OK");

        }else
            display_show_line(3, "Decode: Failed");
    }

    image_free(&image);
    image_free(&resize);

    display_show_line(9,"p. Previous image");
    display_show_line(10,"n. Next image");
    display_show_line(11,"b. Back to image browsing");
    display_show_line(12,"q. Quit");
    display_show_line(13,"input: ");
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
        case 'p':
            if(0 == browser_select_prev(&app -> browser))
                app_set_current_file_from_browser(app);

            app -> current_page = PAGE_IMAGE_VIEWING;
            break;

        case 'n':
            if(0 == browser_select_next(&app -> browser))
                app_set_current_file_from_browser(app);

            app -> current_page = PAGE_IMAGE_VIEWING;
            break;

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
