#ifndef LUMINAFRAME_PAGE_H
#define LUMINAFRAME_PAGE_H

/*
 * Application page identifiers.
 */
typedef enum{
    /* Main menu */
    PAGE_MAIN_MENU = 0  ,

    /* File browser */
    PAGE_IMAGE_BROWSING ,

    /* Image viewer */
    PAGE_IMAGE_VIEWING  ,

    /* Slideshow */
    PAGE_AUTO_PLAY      ,

    /* Exit */
    PAGE_EXIT           ,
} PageId;

/*
 * Get page name for debug output.
 */
const char *page_get_name(PageId page);

#endif /* LUMINAFRAME_PAGE_H */
