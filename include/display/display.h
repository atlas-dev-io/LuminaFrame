#ifndef LUMINAFRAME_DISPLAY_H
#define LUMINAFRAME_DISPLAY_H

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
