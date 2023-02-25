#ifndef JAKE_GL
#define JAKE_GL

typedef int Errno; // type to indicate that a functions return value is errno

// do-while(0) creates a blcok of code which can be treated as a single statement while 
// still allowing local variables to be declared in the scope.
#define return_defer(value) do {result = (value); goto defer; } while(0)

void jgl_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color);
Errno jgl_save_to_ppm(uint32_t* pixels, size_t width, size_t height, const char* file_path);






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


#endif