#ifndef LUMINAFRAME_BMP_H
#define LUMINAFRAME_BMP_H

#include "image/image.h"

/**
 * @brief Load 24-bit BMP image from file.
 *
 * This is the BMP backend used by image module.
 *
 * @param path BMP file path.
 * @param image Image object to store decoded data.
 *
 * @return 0 on success, -1 on failure.
 */
int bmp_load(const char *path, Image *image);

#endif /* LUMINAFRAME_BMP_H */
