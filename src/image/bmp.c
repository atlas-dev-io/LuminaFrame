#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "image/bmp.h"
#include "image/image.h"


#define BMP_FILE_HEADER_SIZE 14
#define BMP_INFO_HEADER_SIZE 40
#define BMP_MAGIC_B          'B'
#define BMP_MAGIC_M          'M'
#define BMP_BI_RGB            0
#define BMP_SUPPORTED_BPP    24


static uint16_t bmp_read_le16(const unsigned char *buf)
{
    return (uint16_t)((uint16_t)buf[0] |
                      ((uint16_t)buf[1] << 8));
}

static uint32_t bmp_read_le32(const unsigned char *buf)
{
    return (uint32_t)((uint32_t)buf[0] |
                      ((uint32_t)buf[1] << 8) |
                      ((uint32_t)buf[2] << 16) |
                      ((uint32_t)buf[3] << 24));
}

static uint32_t bmp_read_le32s(const unsigned char *buf)
{
    return (int32_t)bmp_read_le32(buf);
}

static int bmp_check_header(const unsigned char *file_header,
                            const unsigned char *info_header,
                            int *width,
                            int *height,
                            int *bpp,
                            uint32_t *pixel_offset)
{
    uint32_t  dib_header_size;
    uint16_t  planes         ;
    uint32_t  compression    ;

    if(NULL == file_header || NULL == info_header ||
       NULL == width || NULL == height || NULL ==bpp ||
       NULL == pixel_offset)
        return -1;

    if(BMP_MAGIC_B != file_header[0] || BMP_MAGIC_M != file_header[1])
        return -1;

    *pixel_offset = bmp_read_le32(file_header + 10);

    dib_header_size = bmp_read_le32(info_header + 0);
    if(dib_header_size < BMP_INFO_HEADER_SIZE)
        return -1;

    *width      = bmp_read_le32s(info_header + 4);
    *height     = bmp_read_le32s(info_header + 8);
    planes      = bmp_read_le16(info_header + 12);
    *bpp        = bmp_read_le16(info_header + 14);
    compression = bmp_read_le32(info_header + 16);

    if(0 >= *width)
        return -1;

    /*
     * First version only supports bottom-up BMP.
     * Negative height means top-down BMP, which can be added later.
     */
    if(0 >= *height)
        return -1;

    if(1 != planes)
        return -1;

    if(BMP_SUPPORTED_BPP != *bpp)
        return -1;

    if(BMP_BI_RGB != compression)
        return -1;

    if(*pixel_offset < BMP_INFO_HEADER_SIZE + BMP_FILE_HEADER_SIZE)
        return -1;

    return 0;
}

static int bmp_calc_size(int width,
                         int height,
                         size_t *file_line_bytes,
                         size_t *image_line_bytes,
                         size_t *total_bytes)
{
    size_t raw_line;
  
    if(0 >= width || 0>= height || 
       NULL == file_line_bytes  ||
       NULL == image_line_bytes || 
       NULL == total_bytes)
        return -1;

    raw_line = (size_t)width * 3;

    /*
     * BMP file rows are padded to a 4-byte boundary.
     */
    *file_line_bytes  = (raw_line +3) & ~(size_t)3;
    *image_line_bytes = raw_line                  ;
    *total_bytes      = raw_line * height         ;

    return 0;
}

int bmp_load(const char *path, Image *image)
{
    FILE              *fp                               ;
    unsigned char     file_header[BMP_FILE_HEADER_SIZE] ;
    unsigned char     info_header[BMP_INFO_HEADER_SIZE] ;
    unsigned char     *file_line                        ;
    unsigned char     *dest_line                        ;
    uint32_t          pixel_offset                      ;
    size_t            file_line_bytes                   ;
    size_t            image_line_bytes                  ;
    size_t            total_bytes                       ;
    int               width                             ;
    int               height                            ;
    int               bpp                               ;
    int               y                                 ;
    int               x                                 ;

    if(NULL == path || NULL == image)
        return -1;

    image_init(image);

    fp = fopen(path, "rb");
    if(NULL == fp)
        return -1;

    file_line = NULL;

    if(1 != fread(file_header, 
                  sizeof(file_header), 
                  1, 
                  fp))
        goto fail;

    if(1 != fread(info_header, 
                  sizeof(info_header), 
                  1, 
                  fp))
        goto fail;


    if(0 > bmp_check_header(file_header,
                            info_header, 
                            &width, 
                            &height, 
                            &bpp, 
                            &pixel_offset))
        goto fail;

    if(0 > bmp_calc_size(width,
                         height, 
                         &file_line_bytes, 
                         &image_line_bytes, 
                         &total_bytes))
        goto fail;

    image -> pixels = malloc(total_bytes);
    if(NULL == image -> pixels)
        goto fail;

    file_line = malloc(file_line_bytes);
    if(NULL == file_line)
        goto fail;

    if(0 > fseek(fp, 
                 (long)pixel_offset, 
                 SEEK_SET))
        goto fail;

    for (y = height -1; y >= 0; y--) {
        if(1 != fread(file_line,
                      file_line_bytes,
                      1,
                      fp))
            goto fail;

        dest_line = image -> pixels + y * file_line_bytes;

        for (x = 0; x < width; x++) {

            /*
             * BMP stores 24-bit pixels as BGR.
             * Image stores pixels as RGB.
             */
            dest_line[x * 3 + 0] = file_line[x * 3 + 2];
            dest_line[x * 3 + 1] = file_line[x * 3 + 1];
            dest_line[x * 3 + 2] = file_line[x * 3 + 0];
        }
    }

    image -> width        = width             ;
    image -> height       = height            ;
    image -> bpp          = bpp               ;
    image -> line_bytes   = image_line_bytes  ;
    image -> total_bytes  = total_bytes       ;

    free(file_line);
    fclose(fp);
    return 0;

    fail:
        free(file_line);
        fclose(fp);
        image_free(image);
        return -1;
}










