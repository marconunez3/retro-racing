#include "printf.h"
#include "assert.h"
#include "timer.h"
#include "uart.h"
#include "headerFiles/background.h"
#include "gl.h"


#define ALPHA 0xFF
#define ALPHA_OFFSET 24
#define RED_OFFSET 16
#define GREEN_OFFSET 8

extern const unsigned char backgroundData[];

// Author : Chris Gregg
void parseImage() {
    for (int i = 0; i < sizeof(backgroundData); i += 4) {
        color_t pixel = backgroundData[i] << 24 | // alpha
            backgroundData[i + 1] << 16 | // red
            backgroundData[i + 2] << 8 |  // green
            backgroundData[i + 3];        // blue
        int col = (i / 4) % background_WIDTH;
        int row = (i / 4) / background_WIDTH;
        gl_draw_pixel(col, row, pixel);
    }
}
