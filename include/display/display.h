#ifndef LUMINAFRAME_DISPLAY_H
#define LUMINAFRAME_DISPLAY_H

/**
 * @brief Initialize display module.
 *
 * @return 0 on success, -1 on failure.
 */
int display_init(void);

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
