#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//header for bmp image
#pragma pack(push, 2)
typedef struct bmp_header_t {
	//header
	uint16_t bfType;		//0x4D42 here
	uint32_t bfileSize;		//size of file
	uint32_t bfReserved;	//0
	uint32_t bOffBits;		//смещение от начала
	uint32_t biSize;		//structure size
	//array
	uint32_t biWidth;		//image width
	uint32_t biHeight;		//height
	uint16_t biPlanes;		//here 1
	uint16_t biBitCount;	//bit/pixel
	uint32_t biCompression;		//compression
	uint32_t biSizeImage;		//image size
	uint32_t biXPelsPerMeter;	//horizontal alignment
	uint32_t biYPelsPerMeter;	//vertical alignment
	uint32_t biClrUsed;			//определяет количество используемых цветов из таблицы
	uint32_t biClrImportant;	//important colors, 0 here
} bmp_header_t;
#pragma pack(pop)

//rgb for pixels
typedef struct pixel_t {
	unsigned char blue, green, red;
} pixel_t;

//image structure
typedef struct image_t {
	uint32_t width, height;
	struct pixel_t* data;
} image_t;


// codes for reading image
typedef enum read_status_t {
	READ_OK,
	READ_INVALID_SIGNATURE,
	READ_INVALID_BITS,
	READ_INVALID_HEADER 
} read_status_t;

//read image from file
read_status_t from_bmp(FILE* in, image_t* const read);

//create rotated image
image_t rotate(image_t const source);

//code for writing new image
typedef enum write_status_t {
	WRITE_OK,
	WRITE_ERROR
} write_status_t;

//save rotated image to new file
write_status_t to_bmp(FILE* out, image_t const* image, FILE* in);
