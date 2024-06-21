#ifndef PARSE_IMAGE_H
#define PARSE_IMAGE_H


#include <stdint.h>

struct pixel_t{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct pixel_t getPixelData(int x, int y, int width);
void parseImage(int width, int height);


#endif