#ifndef LUMINAFRAME_APP_H
#define LUMINAFRAME_APP_H

#include "page/page.h"


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

    /* TODO: add global state. */
    /* TODO: selected file. */
    /* TODO: current directory. */
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
