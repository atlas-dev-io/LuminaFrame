#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include "display/display.h"

#define DISPLAY_FB_DEVICE         "/dev/fb0"
#define DISPLAY_SIMULATED_WIDTH   800
#define DISPLAY_SIMULATED_HEIGHT  480
#define DISPLAY_SIMULATED_BPP     32
#define DISPLAY_INVALID_FD        -1

typedef struct{

    /* File descriptor of the display device */
    int           fd            ;

    /* Visible screen width */
    int           width         ;

    /* Visible screen height */
    int           height        ;

    /* Bits per pixel */
    int           bpp           ;

    /* Number of bytes per line in the framebuffer */
    int           line_length   ;

    /* Whether to use the framebuffer */
    int           is_framebuffer;

    /* The virtual address of the video memory obtained after mmap */
    unsigned char *fb_mem       ;

    /* mmap mapping length */
    size_t        fb_size       ;

    /* Framebuffer red component layout */
    int           red_offset    ;
    int           red_length    ;

     /* Framebuffer green component layout */
    int           green_offset  ;
    int           green_length  ;

    /* Framebuffer blue component layout */
    int           blue_offset   ;
    int           blue_length   ;
} DisplayState;

static DisplayState g_display;

static void display_set_simulated_state(void)
{
    g_display.fd              = DISPLAY_INVALID_FD            ;
    g_display.width           = DISPLAY_SIMULATED_WIDTH       ;
    g_display.height          = DISPLAY_SIMULATED_HEIGHT      ;
    g_display.bpp             = DISPLAY_SIMULATED_BPP         ;
    g_display.line_length     = DISPLAY_SIMULATED_WIDTH *
                                (DISPLAY_SIMULATED_BPP  / 8)  ;
    g_display.is_framebuffer  = 0                             ;
    g_display.fb_mem          = NULL                          ;
    g_display.fb_size         = 0                             ;
    g_display.red_offset      = 16                            ;
    g_display.red_length      = 8                             ;
    g_display.green_offset    = 8                             ;
    g_display.green_length    = 8                             ;
    g_display.blue_offset     = 0                             ;
    g_display.blue_length     = 8                             ;
}

static void display_close_framebuffer(void)
{
    if(NULL != g_display.fb_mem){
        munmap(g_display.fb_mem, g_display.fb_size);

        g_display.fb_mem  = NULL;
        g_display.fb_size = 0   ;
    }

    if(0 <= g_display.fd){
        close(g_display.fd);
        g_display.fd = DISPLAY_INVALID_FD;
    }

    g_display.is_framebuffer = 0;
}


static void display_clear_terminal(void)
{
    /*
     * ANSI escape sequence:
     * \033[2J clears the screen.
     * \033[H moves cursor to top-left.
     */
    printf("\033[2J");
    printf("\033[H");
}


static void display_clear_framebuffer(void)
{
    if(!g_display.is_framebuffer)
        return;

    if(NULL == g_display.fb_mem || 0 == g_display.fb_size)
        return;

     /*
     * Clear framebuffer to black.
     *
     * Writing zero works for common RGB framebuffer formats:
     * RGB565  : 0x0000
     * RGB888  : 0x000000
     * XRGB8888: 0x00000000
     *
     * Later display_clear_color() may support custom colors.
     */
    memset(g_display.fb_mem, 0, g_display.fb_size);
}

static unsigned int display_color_to_field(unsigned char value,
                                           int length)
{
    if(0 >= length)
        return 0;

    if(8 <= length)
        return (unsigned int )value << (length - 8);

    return (unsigned int)value >> (8 - length);
}

static unsigned int display_make_pixel(unsigned char r,
                                       unsigned char g,
                                       unsigned char b)
{
    unsigned int pixel = 0;

    pixel |= display_color_to_field(r, g_display.red_length)
          << g_display.red_offset;

    pixel |= display_color_to_field(g, g_display.green_length)
          << g_display.green_offset;

    pixel |= display_color_to_field(b, g_display.blue_length)
          << g_display.blue_offset;

    return pixel;
}

int display_init(void)
{
    int                       fd      ;
    size_t                    fb_size ;
    struct  fb_var_screeninfo var     ;
    struct  fb_fix_screeninfo fix     ;

    /*
     * Start with terminal simulated display.
     *
     * If framebuffer initialization fails, the program can still run
     * on PC terminal for logic testing.
     */
    display_set_simulated_state();

    fd = open(DISPLAY_FB_DEVICE, O_RDWR);
    if(0 > fd){
        printf("[display] framebuffer %s not found,"
               "use simulated display %dx%d\n",
               DISPLAY_FB_DEVICE,
               g_display.width,
               g_display.height);

        return 0;
    }

    if(0 > ioctl(fd, FBIOGET_VSCREENINFO, &var)){
        printf("[display] FBIOGET_VSCREENINFO failed,"
               "use simulated display\n");
        close(fd);
        return 0;
    }

    if(0 > ioctl(fd, FBIOGET_FSCREENINFO, &fix)){
        printf("[display] FBIOGET_FSCREENINFO failed,"
               "use simulated display\n");
        close(fd);
        return 0;
    }

    if(0 == fix.line_length || 0 == var.yres_virtual){
        printf("[display] invalid framebuffer size,"
               "use simulated display\n");
        close(fd);
        return 0;
    }

    /*
     * Map current virtual screen area.
     *
     * Do not use width * bytes_per_pixel here.
     * Framebuffer line length may contain padding bytes.
     */
    fb_size = (size_t)fix.line_length * (size_t)var.yres_virtual;
    if(0 == fb_size){
        printf("[display] framebuffer mmap size is zero,"
               "use simulated display\n");
        close(fd);
        return 0;
    }

    g_display.fb_mem = mmap(NULL,
                            fb_size,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd,
                            0);
    if(MAP_FAILED == g_display.fb_mem){
        g_display.fb_mem = NULL;
        printf("[display] mmap framebuffer failed,"
               "use simulated display\n");
        close(fd);
        return 0;
    }

    g_display.fd              = fd                      ;
    g_display.width           = (int)var.xres           ;
    g_display.height          = (int)var.yres           ;
    g_display.bpp             = (int)var.bits_per_pixel ;
    g_display.line_length     = (int)fix.line_length    ;
    g_display.is_framebuffer  = 1                       ;
    g_display.fb_size         = fb_size                 ;
    g_display.red_offset      = (int)var.red.offset     ;
    g_display.red_length      = (int)var.red.length     ;
    g_display.green_offset    = (int)var.green.offset   ;
    g_display.green_length    = (int)var.green.length   ;
    g_display.blue_offset     = (int)var.blue.offset    ;
    g_display.blue_length     = (int)var.blue.length    ;

    printf("[display] framebuffer: %s, %dx%d, %dbpp,"
           "line_length=%d\n",
           DISPLAY_FB_DEVICE,
           g_display.width,
           g_display.height,
           g_display.bpp,
           g_display.line_length);
    printf("[display] framebuffer mapped: %zu bytes\n",
           g_display.fb_size);

    return 0;
}


int display_get_width(void)
{
    return g_display.width;
}

int display_get_height(void)
{
    return g_display.height;
}

int display_get_bpp(void)
{
    return g_display.bpp;
}

int display_draw_pixel(int x,
                       int y,
                       unsigned char r,
                       unsigned char g,
                       unsigned char b)
{
    unsigned char       *dst            ;
    unsigned int        pixel           ;
    int                 bytes_per_pixel ;

    /*
    * In terminal simulated mode, pixel drawing is a no-op.
    * This keeps PC development working.
    */
    if(!g_display.is_framebuffer)
        return 0;

    if(NULL == g_display.fb_mem)
        return -1;

    if(0 > y || 0 > x)
        return -1;

    if(x >= g_display.width || y >= g_display.height)
        return -1;

    if(0 >= g_display.bpp)
        return -1;

    bytes_per_pixel = (g_display.bpp + 7) / 8;
    if(0 >= bytes_per_pixel || bytes_per_pixel >4)
        return -1;

    dst = g_display.fb_mem +
          (size_t)y * (size_t)g_display.line_length +
          (size_t)x * (size_t)bytes_per_pixel;

    pixel= display_make_pixel(r, g, b);

    /*
    * Current target is little-endian Linux framebuffer.
    * Copy low bytes of packed pixel into framebuffer.
    */
    memcpy(dst,
           &pixel,
           (size_t)bytes_per_pixel);

    return 0;
}


int display_draw_rect(int x,
                      int y,
                      int width,
                      int height,
                      unsigned char r,
                      unsigned char g,
                      unsigned char b)
{
    int x_start;
    int y_start;
    int x_end;
    int y_end;
    int col;
    int row;

    if(0 >= width || 0 >= height)
        return -1;

     /*
     * In terminal simulated mode, rectangle drawing is a no-op.
     * This keeps PC development working.
     */
    if(!g_display.is_framebuffer)
        return 0;

    x_start = x         ;
    y_start = y         ;
    x_end   = x + width ;
    y_end   = y + height;

    if(0 >= x_start)
        x_start = 0;

    if(0 >= y_start)
        y_start = 0;

    if(x_end > g_display.width)
        x_end = g_display.width;

    if(y_end > g_display.height)
        y_end = g_display.height;

    if(x_start >= x_end || y_start >= y_end)
        return -1;

    for(row = y_start; row < y_end; row++){
        for(col = x_start; col < x_end; col++){
            display_draw_pixel(col, row, r, g, b);
        }
    }

    return 0;
}



void display_clear(void)
{
    display_clear_framebuffer();

    /*
     * Keep terminal output clean in current development stage.
     *
     * Text UI still uses terminal escape sequences.
     * Framebuffer text rendering will be added later.
     */
    display_clear_terminal();
}

void display_show_line(int row, const char *text)
{
    /*
     * ANSI escape sequence:
     * \033[%d;1H moves cursor to row, column 1.
     *
     * Terminal row starts from 1, so row + 1 is used.
     */
    printf("\033[%d;1H", row + 1);
    printf("%s", text);
}


void display_refresh(void)
{
    fflush(stdout);
}

void display_close(void)
{
    display_close_framebuffer();

    printf("\033[0m");
    fflush(stdout);
}

