#ifndef LUMINAFRAME_DISPLAY_H
#define LUMINAFRAME_DISPLAY_H

#include <stddef.h>


#define DISPLAY_FB_DEVICE         "/dev/fb0"
#define DISPLAY_SIMULATED_WIDTH   800
#define DISPLAY_SIMULATED_HEIGHT  480
#define DISPLAY_SIMULATED_BPP     32
#define DISPLAY_INVALID_FD        -1

typedef struct{

    /* File descriptor of the display device */
    int           fd            ;

    /* Visible screen width */
    int           width         ;

    /* Visible screen height */
    int           height        ;

    /* Bits per pixel */
    int           bpp           ;

    /* Number of bytes per line in the framebuffer */
    int           line_length   ;

    /* Whether to use the framebuffer */
    int           is_framebuffer;

    /* The virtual address of the video memory obtained after mmap */
    unsigned char *fb_mem       ;

    /* mmap mapping length */
    size_t        fb_size       ;
} DisplayState;


/**
 * @brief Initialize display module.
 *
 * @return 0 on success, -1 on failure.
 */
int display_init(void);


/**
 * @brief Get display width.
 *
 * Terminal version returns simulated screen width.
 * Framebuffer version will return real LCD width.
 *
 * @return Display width in pixels.
 */
int display_get_width(void);

/**
 * @brief Get display height.
 *
 * Terminal version returns simulated screen height.
 * Framebuffer version will return real LCD height.
 *
 * @return Display height in pixels.
 */
int display_get_height(void);

/**
 * @brief Get display bits per pixel.
 *
 * @return Display bits per pixel.
 */
int display_get_bpp(void);

/**
 * @brief Clear display content.
 */
void display_clear(void);


/**
 * @brief Show one line of text.
 *
 * @param row Line number.
 * @param text Text to show.
 */
void display_show_line(int row, const char *text);

/**
 * @brief Refresh display content.
 */
void display_refresh(void);

/**
 * @brief Close display module.
 */
void display_close(void);


#endif /* LUMINAFRAME_DISPLAY_H */
