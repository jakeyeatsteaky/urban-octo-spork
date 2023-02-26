#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "jake_gl.cpp"

#define WIDTH 800
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

void jgl_make_checker_board(uint32_t* pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t cb_w, size_t cb_h, uint32_t color1, uint32_t color2, int grid)
{    
    //TODO:
    //  - handle odd number of squares
    int w_offset = cb_w % grid;
    int h_offset = cb_h % grid;
    if(w_offset)
        cb_w -= w_offset;
    if(h_offset)
        cb_h -= h_offset;

    bool switchColor = false;
    int cb_counter_y = 0;
    int cb_pattern_y = cb_h/grid;
    for(int dy = 0; dy < (int) cb_h; ++dy) {
        int y = y0 + dy;
        if(0 <= y && y < (int) pixels_height) {
            int cb_counter_x = 0;
            int cb_pattern_x = cb_w/grid;
            for(int dx = 0; dx < (int) cb_w; ++dx) {
                int x = x0 + dx;
                if(0 <= x && x < (int) pixels_width) {
                    if(switchColor)
                        pixels[y * pixels_width + x] = color2;
                    else   
                        pixels[y * pixels_width + x] = color1;
                    cb_counter_x++;
                    if(cb_counter_x >= cb_pattern_x) {
                        switchColor = !switchColor;
                        cb_counter_x = 0;
                    }
                }
            }
        }
        cb_counter_y++;
        if(cb_counter_y >= cb_pattern_y) {
            switchColor = !switchColor;
            cb_counter_y = 0;
        }
    }   
}

void jgl_make_checker_board2(uint32_t* pixels, size_t pixels_width, size_t pixels_height, uint32_t color1, uint32_t color2, size_t rows, size_t cols)
{   
    size_t cell_width = pixels_width / cols;
    size_t cell_height = pixels_height / rows;

    for(int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            uint32_t color;
            if((x+y)%2 == 0)
                color = color1;
            else
                color = color2;
            jgl_fill_rect(pixels, pixels_width, pixels_height, x*cell_width, y*cell_height, cell_width, cell_height, color);
        }
    } 
}


int main(void) 
{
    const char* file_path = "output.ppm";
    jgl_fill(pixels, WIDTH, HEIGHT, 0xBEEFBEEF);    // 0xAABBGGRR
    size_t rect_w = 200*2.5;
    size_t rect_h = 200*2.5;
    size_t cb_w = 450;
    size_t cb_h = 450;
    jgl_fill_rect(pixels, WIDTH, HEIGHT, WIDTH/2 - (rect_w/2), HEIGHT/2 - (rect_h/2), rect_w, rect_h, 0xFFFFFFFF);
    jgl_fill_rect_center(pixels, WIDTH, HEIGHT, WIDTH/2 +5, HEIGHT/2+5, rect_w, rect_h, 0xFF0000FF);
    jgl_make_checker_board2(pixels, WIDTH, HEIGHT, 0xFF0000FF, 0xFFFF0000, 6,8);
    jgl_make_checker_board(pixels, WIDTH, HEIGHT, WIDTH/2 - cb_w/2, HEIGHT/2 - cb_h/2, cb_w, cb_h, 0xFFFFFFFF, 0xFF000000,8);
    Errno err = jgl_save_to_ppm(pixels, WIDTH, HEIGHT, file_path);
    if(err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return 1;
    }

}




