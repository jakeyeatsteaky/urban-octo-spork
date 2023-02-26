#ifndef JAKE_GL
#define JAKE_GL

typedef int Errno; // type to indicate that a functions return value is errno

// do-while(0) creates a blcok of code which can be treated as a single statement while 
// still allowing local variables to be declared in the scope.
#define return_defer(value) do {result = (value); goto defer; } while(0)

void jgl_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color);
Errno jgl_save_to_ppm(uint32_t* pixels, size_t width, size_t height, const char* file_path);
void jgl_fill_rect(uint32_t* pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t w, size_t h, uint32_t color);
void jgl_fill_rect_center(uint32_t* pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t w, size_t h, uint32_t color);
void jgl_make_checker_board(uint32_t* pixels, size_t pixels_width, size_t pixels_height, int x0, int y0, size_t cb_w, size_t cb_h, uint32_t color1, uint32_t color2, int grid);
void jgl_make_checker_board2(uint32_t* pixels, size_t pixels_width, size_t pixels_height, uint32_t color1, uint32_t color2, size_t rows, size_t cols);





void jgl_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color)
{

    for(size_t y = 0; y < height; ++y) {
        for(size_t x = 0; x < width; ++x) {
            pixels[y * width + x] = color;
        }
    }
}

Errno jgl_save_to_ppm(uint32_t* pixels, size_t width, size_t height, const char* filepath)
{
    // header and footer paradigm setup to be able to close multiple resources if need be
    int result = 0;
    FILE* f = NULL;

    {
        ////////////////////////////////////////////////////////////////
        //   This function takes the pixel buffer whose color was     //
        //  specified in the jgl_fill function, and writes that pixel // 
        //  binary data to the PPM file                               //
        ////////////////////////////////////////////////////////////////

        f = fopen(filepath, "wb");                      // open file for writing
        if(f == NULL) return_defer(errno);

        fprintf(f, "P6\n%zu %zu 255\n", width, height); // create PPM header
        if(ferror(f)) return_defer(errno);

        for(size_t i = 0; i < width*height; ++i) {      // iterate through the buffer and write pixel data
                                                        // 0xAABBGGRR -> RGB configuration 32 bit
            uint32_t pixel = pixels[i];
            uint8_t bytes[3] = {
                (pixel >> (8*0)) & 0xFF,                  // shift by 0 bytes to extract only RR of 0xAABBGGRR
                (pixel >> (8*1)) & 0xFF,                  // shift by 1 byte to extract only GG of 0xAABBGGRR
                (pixel >> (8*2)) & 0xFF                   // shift by 2 bytes to extract only BB of 0xAABBGGRR
            };          
            fwrite(bytes, sizeof(bytes), 1, f);
            if(ferror(f)) return_defer(errno);
        }
    }

defer:
    if(f) fclose(f);
    return result;

}

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

    for(size_t y = 0; y < rows; ++y) {
        for (size_t x = 0; x < cols; ++x) {
            uint32_t color;
            if((x+y)%2 == 0)
                color = color1;
            else
                color = color2;
            jgl_fill_rect(pixels, pixels_width, pixels_height, x*cell_width, y*cell_height, cell_width, cell_height, color);
        }
    } 
}

#endif