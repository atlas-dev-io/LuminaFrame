#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include "browser/browser.h"

static int browser_is_skip_name(const char *name)
{
    if(NULL == name)
        return 1;

    if(0 == strcmp(name, "."))
        return 1;

    if(0 == strcmp(name, ".."))
        return 1;

    return 0;
}

static int browser_is_regular_file(const char *dir_path, const char *name)
{
    char full_path[BROWSER_MAX_PATH_LEN];
    struct stat st;
    int ret;

    ret = snprintf(full_path, 
                   sizeof(full_path),
                   "%s/%s",
                   dir_path,
                   name);
    if(ret < 0 || (size_t)ret >=sizeof(full_path))
        return 0;
    if(stat(full_path, &st))
        return 0;

    return S_ISREG(st.st_mode);
}

static int browser_is_image_file(const char *name)
{
    const char *dot;
    if(NULL == name)
        return 0;

    dot = strrchr(name, '.');
    if(NULL == dot)
        return 0;

    if(name == dot)
        return 0;

    if('\0' == dot[1])
        return 0;

    if(0 == strcasecmp(dot, ".bmp"))
        return 1;

    if(0 == strcasecmp(dot, ".jpg"))
        return 1;

    if(0 == strcasecmp(dot, ".jpeg"))
        return 1;

    return 0;
}

static int browser_compare_entry(const void *left, const void *right)
{
    const BrowserEntry *left_entry  = left  ;
    const BrowserEntry *right_entry = right ;

    int ret = strcasecmp(left_entry -> name, right_entry -> name);
    if(0 != ret)
        return ret;

    return strcmp(left_entry -> name, right_entry -> name);
}

static void browser_sort_entries(BrowserState *browser)
{
    if(NULL == browser)
        return;

    if(browser ->entry_count <= 1)
        return;

    qsort(browser -> entries, 
          browser -> entry_count, 
          sizeof(browser -> entries[0]), 
          browser_compare_entry);
}

void browser_init(BrowserState *browser)
{
    if(NULL == browser)
        return ;

    browser -> current_dir[0] = '\0';
    browser -> entry_count    =   0 ;
    browser -> selected_index =   0 ;

    memset(browser -> entries, 0, sizeof(browser -> entries));
}

int browser_load_dir(BrowserState *browser, const char *dir_path)
{
    DIR *dir;
    struct dirent *entry;

    if(NULL == browser || NULL == dir_path)
        return -1;

    browser_init(browser);

    snprintf(browser -> current_dir,
             sizeof(browser -> current_dir),
             "%s",
             dir_path);

    dir = opendir(dir_path);
    if(NULL == dir)
        return -1;

    while(NULL != (entry = readdir(dir))){
        if(browser -> entry_count >= BROWSER_MAX_ENTRIES)
            break;

        if(browser_is_skip_name(entry -> d_name))
            continue;

        if(!browser_is_regular_file(dir_path, entry -> d_name))
            continue;

        if(!browser_is_image_file(entry -> d_name))
            continue;

        snprintf(browser -> entries[browser -> entry_count].name,
             sizeof(browser->entries[browser->entry_count].name),
             "%s",
             entry -> d_name);

        browser -> entry_count++;
    }

    closedir(dir);
    browser_sort_entries(browser);

    return 0;
}


size_t browser_get_count(const BrowserState *browser)
{
    if(NULL == browser)
        return 0;

    return browser ->entry_count;
}


const char *browser_get_entry(const BrowserState *browser, size_t index)
{
    if(NULL == browser)
        return NULL;

    if(index >= browser ->entry_count)
        return NULL;

    return browser -> entries[index].name;
}

