#ifndef LUMINAFRAME_IMAGE_H
#define LUMINAFRAME_IMAGE_H

#include <stddef.h>


/**
 * @brief Decoded image object.
 *
 * The first version stores decoded RGB pixel data.
 * Future versions may add pixel format, alpha channel, or source type.
 */
typedef struct{
    int               width       ;
    int               height      ;
    int               bpp         ;
    size_t            line_bytes  ;
    size_t            total_bytes ;
    unsigned char*    pixels      ;
} Image;


/**
 * @brief Initialize image object.
 *
 * @param image Image object.
 */
void image_init(Image *image);

/**
 * @brief Load image from file.
 *
 * The first version only supports 24-bit BMP files.
 *
 * @param path Image file path.
 * @param image Image object to store decoded data.
 *
 * @return 0 on success, -1 on failure.
 */
int image_load(const char *path, Image *image);

/**
 * @brief Free decoded image data.
 *
 * @param image Image object.
 */
void image_free(Image *image);


#endif /* LUMINAFRAME_IMAGE_H */
