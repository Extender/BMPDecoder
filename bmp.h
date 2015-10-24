#ifndef BMP_H
#define BMP_H

// Supports the following Windows BMP formats:
// 1 bpp, 4 bpp, 8 bpp, 16 bpp (all variations), 32 bpp (all variations)
// RLE4bpp, RLE8bpp

// Supports the following compression formats:

#define BMP_COMPRESSION_RGB 0
#define BMP_COMPRESSION_RLE8 1
#define BMP_COMPRESSION_RLE4 2
#define BMP_COMPRESSION_BITFIELDS 3

#include <stdlib.h>
#include <math.h>
#include "io.h" // Taken from "DefaultLibrary"

class bmp
{
public:
    static uint32_t *decode(char *data, int32_t &width, int32_t &height);
};

#endif // BMP_H
