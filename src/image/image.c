#include <stdlib.h>
#include <stddef.h>

#include "image/image.h"
#include "image/bmp.h"

#define IMAGE_RGB_BPP 24
#define IMAGE_RGB_BYTES_PER_PIXEL 3


static int image_calc_fit_size(const Image *src,
                               int max_width,
                               int max_height,
                               int *dst_width,
                               int *dst_height)
{
    long width_by_height;
    long height_by_width;

    if(NULL == src || NULL == dst_width || NULL == dst_height)
        return -1;

    if(0 >= src -> width || 0 >= src -> height)
        return -1;

    if(0 >= max_width || 0 >= max_height)
        return -1;

    width_by_height = (long)max_height * src -> width / src -> height;

     /*
     * Keep aspect ratio.
     *
     * There are two possible ways:
     * 1. Use max_width as limit, then calculate height.
     * 2. Use max_height as limit, then calculate width.
     *
     * Pick the one that can fit inside max_width x max_height.
     */
    if(width_by_height <= max_width){
        *dst_width  = (int)width_by_height;
        *dst_height = max_height          ;
    }else{
        height_by_width = (long)max_width * src -> height / src -> width;

        *dst_height = (int)height_by_width;
        *dst_width  = max_width           ;
    }

    if(0 >= *dst_height)
        *dst_height = 1;

    if(0 >= *dst_width)
        *dst_width  = 1;

    return 0;
}

static int image_resize_nearest(const Image *src,
                                int dst_width,
                                int dst_height,
                                Image *dst)
{
    unsigned char       *src_pixel    ;
    unsigned char       *dst_pixel    ;
    size_t              src_index     ;
    size_t              dst_index     ;
    int                 src_x         ;
    int                 src_y         ;
    int                 x             ;
    int                 y             ;

    if(NULL ==src || NULL == dst)
        return -1;

    if(NULL == src -> pixels)
        return -1;

    if(IMAGE_RGB_BPP != src -> bpp)
        return -1;

    if(0 >= src -> width || 0 >= src -> height)
        return -1;

    if(0 >= dst_width || 0 >= dst_height)
        return -1;

    image_init(dst);

    dst -> width        = dst_width                                     ;
    dst -> height       = dst_height                                    ;
    dst -> bpp          = IMAGE_RGB_BPP                                 ;
    dst -> line_bytes   = (size_t)dst_width * IMAGE_RGB_BYTES_PER_PIXEL ;
    dst -> total_bytes  = dst -> line_bytes * (size_t)dst_height        ;

    dst -> pixels       = malloc(dst -> total_bytes)              ;
    if(NULL == dst -> pixels){
        image_init(dst);
        return -1;
    }

    for(y = 0; y < dst -> height; y++){

      /*
      * Nearest-neighbor sampling:
      * map current destination row back to source row.
      */
      src_y = y * src -> height / dst_height;

      for(x = 0; x < dst -> width; x++){

          /*
          * Nearest-neighbor sampling:
          * map current destination column back to source column.
          */
          src_x = x * src -> width / dst_width;

          src_index = (size_t)src_y * src -> line_bytes +
                      (size_t)src_x * IMAGE_RGB_BYTES_PER_PIXEL;

          dst_index = (size_t)y * dst -> line_bytes +
                      (size_t)x * IMAGE_RGB_BYTES_PER_PIXEL;

          src_pixel = src -> pixels + src_index;
          dst_pixel = dst -> pixels + dst_index;

          /*
          * Current Image stores pixels as RGB.
          */
          dst_pixel[0] = src_pixel[0];
          dst_pixel[1] = src_pixel[1];
          dst_pixel[2] = src_pixel[2];
      }
  }
    return 0;
}

int image_resize_to_fit(const Image *src, int max_width, int max_height, Image *dst)
{
    int dst_width;
    int dst_height;

    if(NULL == src || NULL == dst)
        return -1;

    if(0 > image_calc_fit_size(src,
                               max_width,
                               max_height,
                               &dst_width,
                               &dst_height))
        return -1;

    return image_resize_nearest(src,
                                dst_width,
                                dst_height,
                                dst);
}


void image_init(Image *image)
{
    if(NULL == image)
        return;

    image -> width       = 0;
    image -> height      = 0;
    image -> bpp         = 0;
    image -> line_bytes  = 0;
    image -> total_bytes = 0;
    image -> pixels      = NULL;
}

int image_load(const char *path, Image *image)
{
    if(NULL == image || NULL == path)
        return -1;

    image_init(image);

    /*
     * The first image backend only supports BMP.
     * Later image_load() may dispatch by file magic or extension.
     */
    return bmp_load(path, image);
}


void image_free(Image *image)
{
    if(NULL == image)
        return;

    free(image -> pixels);
    image_init(image);
}















