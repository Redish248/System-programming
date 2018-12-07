#include <stdint.h>
#include "image_sepia.h"
#include "picture.h"
#include <inttypes.h>


static unsigned char sat(uint64_t x) {
    if (x < 256) return x; return 255;
}

static void sepia_one(pixel_t* const pixel ) {
    static const float c[3][3] =  {
            { .393f, .769f, .189f },
            { .349f, .686f, .168f },
            { .272f, .543f, .131f } };
    pixel_t const old = *pixel;
    pixel->red = sat(old.red * c[0][0] + old.green * c[0][1] + old.blue * c[0][2]);
    pixel->green = sat(old.red * c[1][0] + old.green * c[1][1] + old.blue  * c[1][2]);
    pixel->blue = sat(old.red * c[2][0] + old.green * c[2][1] + old.blue * c[2][2]);
}

void sepia_c_inplace( image_t* img ) {
    uint32_t x,y;
    uint32_t height = img->height;
    uint32_t width = img->width;

    for( y = 0; y < height; y++ ) {
        for (x = 0; x < width; x++) {
            sepia_one(img->data + y * width + x);
        }
    }
}

void image_sepia_sse(image_t* image) {
    float blue[4];
    float green[4];
    float red[4];
    uint8_t result[12];

    size_t width = image->width;
    size_t height = image->height;

    pixel_t* pixel;

    for (size_t i = 0; i < width*height; i+=4) {
        for (size_t j = 0; j < 4; j++){
            pixel = image->data + i + j;
            blue[j]  = pixel->blue;
            red[j] = pixel->red;
            green[j] = pixel->green;
        }
        sepia_sse(blue,green,red,result);
        for (size_t j = 0; j < 4; j++) {
            pixel = image->data + i + j;
            pixel->blue = *(result+3*j);
            pixel->red = *(result+3*j + 1);
            pixel->green = *(result+3*j + 2);
        }
    }
}



