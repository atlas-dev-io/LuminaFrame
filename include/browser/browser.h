#ifndef LUMINAFRAME_BROWSER_H
#define LUMINAFRAME_BROWSER_H

#include <stddef.h>

#define BROWSER_MAX_ENTRIES 128
#define BROWSER_MAX_NAME_LEN 256
#define BROWSER_MAX_PATH_LEN 512


/**
 * @brief Browser entry object.
 *
 * The first version only stores file name. Future versions may add
 * file type, file size, modified time, or full path.
 */
typedef struct{
    char name[BROWSER_MAX_NAME_LEN];
} BrowserEntry;


/**
 * @brief Browser module state.
 *
 * BrowserState stores current directory and scanned file list.
 * It does not display files and does not operate framebuffer.
 */
typedef struct{
    /* Currently scanned directory */
    char current_dir[BROWSER_MAX_PATH_LEN];

    /* File list */
    BrowserEntry entries[BROWSER_MAX_ENTRIES];

    /* Number of files */
    size_t entry_count;

    /* Currently selected item */
    size_t selected_index;
}BrowserState;

/**
 * @brief Initialize browser state.
 *
 * @param browser Browser state object.
 */
void browser_init(BrowserState *browser);


/**
 * @brief Load image files from directory.
 *
 * Scan the specified directory, skip "." and "..", and save image
 * file names into browser state.
 *
 * @param browser Browser state object.
 * @param dir_path Directory path to scan.
 *
 * @return 0 on success, -1 on failure.
 */
int browser_load_dir(BrowserState *browser, const char *dir_path);

/**
 * @brief Get file entry count.
 *
 * @param browser Browser state object.
 *
 * @return Number of scanned file entries.
 */
size_t browser_get_count(const BrowserState *browser);

/**
 * @brief Get file name by index.
 *
 * @param browser Browser state object.
 * @param index File entry index.
 *
 * @return File name string, or NULL if index is invalid.
 */
const char *browser_get_entry(const BrowserState *browser, size_t index);


#endif /* LUMINAFRAME_BROWSER_H */
