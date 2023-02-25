#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "jake_gl.cpp"

#define WIDTH 600
#define HEIGHT 600

static uint32_t pixels[WIDTH * HEIGHT];

void jgl_fill_rect(uint32_t* pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t w, size_t h, uint32_t color)
{
    for(int dy = 0; dy < (int) h; ++dy) {
        int y = y0 + dy;
        if(0 <= y && y < (int) pixels_height) {
            for(int dx = 0; dx < (int) w; ++dx) {
                int x = x0 + dx;
                if(0 <= x && x < (int) pixels_width) {
                    pixels[y * pixels_width + x] = color;
                }
            }

        }
    }
}
void jgl_fill_rect_center(uint32_t* pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t w, size_t h, uint32_t color)
{
    for(int dy = 0; dy < (int) h; ++dy) {
        int y = (y0 - h/2) + dy;
        if(0 <= y && y < (int) pixels_height) {
            for(int dx = 0; dx < (int) w; ++dx) {
                int x = (x0 - w/2) + dx;
                if(0 <= x && x < (int) pixels_width) {
                    pixels[y * pixels_width + x] = color;
                }
            }

        }
    }
}

int main(void) 
{
    const char* file_path = "output.ppm";
    jgl_fill(pixels, WIDTH, HEIGHT, 0xFF000000);    // 0xAABBGGRR
    size_t rect_w = 200;
    size_t rect_h = 200;
    jgl_fill_rect(pixels, WIDTH, HEIGHT, WIDTH/2 - (rect_w/2), HEIGHT/2 - (rect_h/2), 100, 100, 0xFFFFFFFF);
    Errno err = jgl_save_to_ppm(pixels, WIDTH, HEIGHT, file_path);
    if(err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return 1;
    }

}




