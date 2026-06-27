#ifndef LUMINAFRAME_APP_H
#define LUMINAFRAME_APP_H

#include "page/page.h"
#include "browser/browser.h"


#define APP_MAX_PATH_LEN 512


/*
 * Global application state.
 *
 * All pages communicate through this structure.
 */
typedef struct{
    /* Current active page. */
    PageId current_page;

    /** Current selected file path. */
    char current_file_path[APP_MAX_PATH_LEN];

    /*
     * Browser state shared by image browsing page and image viewing page.
     *
     * It keeps scanned image list, current directory, and selected index
     * across page switching.
     */
    BrowserState browser;

    /* TODO: add global state. */
    /* TODO: user input event. */
} AppState;


/**
 * @brief Initialize application state.
 *
 * @param app Application state object.
 */
void app_init(AppState *app);


/**
 * @brief Run application main loop.
 *
 * @param app Application state object.
 */
void app_run(AppState *app);


#endif /* LUMINAFRAME_APP_H */
