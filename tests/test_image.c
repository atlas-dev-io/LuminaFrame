
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

int main(void)
{
    test_load_aligned_bmp();
    test_load_padded_bmp();
    test_reject_non_image();

    printf("test_image: OK\n");
    return 0;
}
