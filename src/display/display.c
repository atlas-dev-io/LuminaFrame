#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>

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

void display_clear(void)
{
/*
     * ANSI escape sequence:
     * \033[2J clears the screen.
     * \033[H moves cursor to top-left.
     */
    printf("\033[2J");
    printf("\033[H");
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

