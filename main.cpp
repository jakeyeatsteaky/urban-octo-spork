#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <cmath>
#include <string.h>
#include "jake_gl.cpp"

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND_COLOR 0xFF252525

static uint32_t pixels[WIDTH * HEIGHT];

// TODO: lines
//      https://www.youtube.com/watch?v=LmQKZmQh1ZQ&t=4683s
//  @ 1:21:03

void jgl_draw_line(uint32_t* pixels, size_t pixelsWidth, size_t pixels_height, int x1, int y1, int x2, int y2, uint32_t color)
{
    //                 y = mx + b - find the m and b
    //       y1 - m * x1 = b
    //                y2 = mx2 + y1 - mx1
    //           y2 - y1 = m (x2 - x1)
    // y2 - y1 / x2 - x1 = m
    // y1 - m * x1 = b

    int dx = x2 - x1;
    int dy = y2 - y1;
    if(dx != 0){
        int slope = dy / dx;
        int b = y1 - slope*x1;
    } else {
        
    }

}

int main(void) 
{
    jgl_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);    // 0xAABBGGRR
    size_t rect_w = 200*2.5;
    size_t rect_h = 200*2.5;
    size_t cb_w = 450;
    size_t cb_h = 450;
    jgl_fill_rect(pixels, WIDTH, HEIGHT, WIDTH/2 - (rect_w/2), HEIGHT/2 - (rect_h/2), rect_w, rect_h, 0xFFFFFFFF);
    jgl_fill_rect_center(pixels, WIDTH, HEIGHT, WIDTH/2 +5, HEIGHT/2+5, rect_w, rect_h, 0xFF0000FF);
    jgl_make_checker_board2(pixels, WIDTH, HEIGHT, 0xFFFF0000, 0xFF0000FF, 6,8);
    jgl_make_checker_board(pixels, WIDTH, HEIGHT, WIDTH/2 - cb_w/2, HEIGHT/2 - cb_h/2, cb_w, cb_h, 0xFFFFFFFF, 0xFF000000,8);
    jgl_fill_circle(pixels, WIDTH, HEIGHT, WIDTH/2, HEIGHT/2, 100, 0xFFFF00FF);
    jgl_fill_circle(pixels, WIDTH, HEIGHT, WIDTH/2, HEIGHT/2, 100, 0xFFFF00FF);
    jgl_fill_triangle(pixels, WIDTH, HEIGHT, 100, 100, WIDTH/2, HEIGHT/2, 0xFFFF0000); 
    jgl_circle_gradient(pixels, WIDTH, HEIGHT, 0xFF0000FF, 50);
    const char* file_path = "output.ppm";
    Errno err = jgl_save_to_ppm(pixels, WIDTH, HEIGHT, file_path);
    if(err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return 1;
    }

}




