CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -I./include
LDFLAGS =

TARGET = lumina_frame
BUILD_DIR = build
TEST_IMAGE_TARGET = test_image
ARM_CC ?= arm-linux-gnueabihf-gcc
ARM_TARGET = lumina_frame_arm
ARM_STATIC_LDFLAGS = -static

SRCS = main.c \
       src/app/app.c \
       src/page/page.c \
       src/page/main_menu_page.c \
       src/page/image_browsing_page.c \
       src/page/image_viewing_page.c \
       src/page/auto_play_page.c \
       src/display/display.c \
       src/input/input.c \
       src/browser/browser.c \
       src/image/image.c \
       src/image/bmp.c \

TEST_IMAGE_SRCS = tests/test_image.c \
                  src/image/image.c \
                  src/image/bmp.c

.PHONY: all run arm-static test test-image clean

all:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET)

run: all
	./$(BUILD_DIR)/$(TARGET)

arm-static:
	mkdir -p $(BUILD_DIR)
	$(ARM_CC) $(CFLAGS) $(SRCS) $(ARM_STATIC_LDFLAGS) -o $(BUILD_DIR)/$(ARM_TARGET)

test-image:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_IMAGE_SRCS) $(LDFLAGS) -o $(BUILD_DIR)/$(TEST_IMAGE_TARGET)
	./$(BUILD_DIR)/$(TEST_IMAGE_TARGET)

test: test-image

clean:
	rm -rf $(BUILD_DIR)
