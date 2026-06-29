#include <assert.h>
#include <stdio.h>

#include "image/image.h"

static void test_load_aligned_bmp(void)
{
    Image image;

    image_init(&image);

    assert(image_load("tests/fixtures/images/aligned_4x2.bmp", &image) == 0);
    assert(image.width == 4);
    assert(image.height == 2);
    assert(image.bpp == 24);
    assert(image.line_bytes == 12);
    assert(image.total_bytes == 24);
    assert(image.pixels != NULL);

    image_free(&image);
    assert(image.pixels == NULL);
}

static void test_load_padded_bmp(void)
{
    Image image;

    image_init(&image);

    assert(image_load("tests/fixtures/images/padded_2x2.bmp", &image) == 0);
    assert(image.width == 2);
    assert(image.height == 2);
    assert(image.bpp == 24);
    assert(image.line_bytes == 6);
    assert(image.total_bytes == 12);
    assert(image.pixels != NULL);

    image_free(&image);
    assert(image.pixels == NULL);
}

static void test_reject_non_image(void)
{
    Image image;

    image_init(&image);

    assert(image_load("tests/fixtures/images/not_image.txt", &image) == -1);
    assert(image.pixels == NULL);

    image_free(&image);
}

static void test_resize_aligned_bmp_to_fit(void)
{
    Image src;
    Image dst;

    image_init(&src);
    image_init(&dst);

    assert(image_load("tests/fixtures/images/aligned_4x2.bmp", &src) == 0);

    /*
     * Source image is 4x2.
     * Fit it into 2x2 while keeping aspect ratio.
     *
     * Expected result:
     * width  = 2
     * height = 1
     */
    assert(image_resize_to_fit(&src, 2, 2, &dst) == 0);
    assert(dst.width == 2);
    assert(dst.height == 1);
    assert(dst.bpp == 24);
    assert(dst.line_bytes == 6);
    assert(dst.total_bytes == 6);
    assert(dst.pixels != NULL);

    image_free(&dst);
    image_free(&src);

    assert(dst.pixels == NULL);
    assert(src.pixels == NULL);
}

static void test_resize_padded_bmp_to_fit(void)
{
    Image src;
    Image dst;

    image_init(&src);
    image_init(&dst);

    assert(image_load("tests/fixtures/images/padded_2x2.bmp", &src) == 0);

    /*
     * Source image is 2x2.
     * Fit it into 1x1 while keeping aspect ratio.
     */
    assert(image_resize_to_fit(&src, 1, 1, &dst) == 0);
    assert(dst.width == 1);
    assert(dst.height == 1);
    assert(dst.bpp == 24);
    assert(dst.line_bytes == 3);
    assert(dst.total_bytes == 3);
    assert(dst.pixels != NULL);

    image_free(&dst);
    image_free(&src);

    assert(dst.pixels == NULL);
    assert(src.pixels == NULL);
}

static void test_resize_reject_invalid_args(void)
{
    Image src;
    Image dst;

    image_init(&src);
    image_init(&dst);

    assert(image_load("tests/fixtures/images/aligned_4x2.bmp", &src) == 0);

    assert(image_resize_to_fit(NULL, 2, 2, &dst) == -1);
    assert(image_resize_to_fit(&src, 0, 2, &dst) == -1);
    assert(image_resize_to_fit(&src, 2, 0, &dst) == -1);
    assert(image_resize_to_fit(&src, 2, 2, NULL) == -1);

    image_free(&dst);
    image_free(&src);
}

int main(void)
{
    test_load_aligned_bmp();
    test_load_padded_bmp();
    test_reject_non_image();

    test_resize_aligned_bmp_to_fit();
    test_resize_padded_bmp_to_fit();
    test_resize_reject_invalid_args();

    printf("test_image: OK\n");
    return 0;
}
