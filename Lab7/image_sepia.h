#include "picture.h"

static unsigned char sat(uint64_t x);
static void sepia_one(pixel_t* const pixel );
void sepia_c_inplace(image_t* img);

void image_sepia_sse(image_t* image);
extern void sepia_sse(float* blue, float* green, float* red, uint8_t* result);