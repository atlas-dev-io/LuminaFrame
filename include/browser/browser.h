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
 * Scan the specified directory, skip "." and "..", filter ordinary image
 * files by extension, sort them by file name, and save file names into
 * browser state.
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


/**
 * @brief Get current scanned directory.
 *
 * @param browser Browser state object.
 *
 * @return Current directory string, or NULL if browser is invalid.
 */
const char *browser_get_current_dir(const BrowserState *browser);

/**
 * @brief Select file entry by index.
 *
 * @param browser Browser state object.
 * @param index File entry index.
 *
 * @return 0 on success, -1 on failure.
 */
int browser_select_entry(BrowserState *browser, size_t index);


/**
 * @brief Select next file entry.
 *
 * @param browser Browser state object.
 *
 * @return 0 on success, -1 if there is no next entry.
 */
int browser_select_next(BrowserState *browser);

/**
 * @brief Select previous file entry.
 *
 * @param browser Browser state object.
 *
 * @return 0 on success, -1 if there is no previous entry.
 */
int browser_select_prev(BrowserState *browser);

/**
 * @brief Get currently selected file name.
 *
 * @param browser Browser state object.
 *
 * @return Selected file name string, or NULL if no valid entry is selected.
 */
const char *browser_get_selected_entry(const BrowserState *browser);


#endif /* LUMINAFRAME_BROWSER_H */
