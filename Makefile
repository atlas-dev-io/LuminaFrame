CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -I./include

TARGET = lumina_frame
BUILD_DIR = build

SRCS = main.c \
       src/app/app.c \
       src/page/page.c \
       src/page/main_menu_page.c \
       src/page/image_browsing_page.c \
       src/page/image_viewing_page.c \
       src/page/auto_play_page.c \
			 src/display/display.c \
			 src/input/input.c \
		   src/browser/browser.c


all:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/$(TARGET)

run: all
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
