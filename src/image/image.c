#include <stdlib.h>
#include <stddef.h>

#include "image/image.h"
#include "image/bmp.h"


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















