


#include "page/page.h"
#include "page/main_menu_page.h"
#include "page/image_browsing_page.h"
#include "page/image_viewing_page.h"
#include "page/auto_play_page.h"


const char *page_get_name(PageId page)
{
    switch (page) {
        case PAGE_MAIN_MENU:
            return "main menu";

        case PAGE_IMAGE_BROWSING:
            return "image browsing";

        case PAGE_IMAGE_VIEWING:
            return "image viewing";

        case PAGE_AUTO_PLAY:
            return "auto play";

        case PAGE_EXIT:
            return "exit";

        default:
            return "unknown";
    }
}
