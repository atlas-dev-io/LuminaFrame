#ifndef LUMINAFRAME_APP_H
#define LUMINAFRAME_APP_H

#include "page/page.h"

/*
 * Global application state.
 *
 * All pages communicate through this structure.
 */
typedef struct{
    /* Current active page. */
    PageId current_page;

    /* TODO: add global state. */
    /* TODO: selected file. */
    /* TODO: current directory. */
    /* TODO: current image path. */
    /* TODO: user input event. */
} AppState;


/*
 * Initialize application state.
 */
void app_init(AppState *app);


/*
 * Run application main loop.
 */
void app_run(AppState *app);


#endif /* LUMINAFRAME_APP_H */
