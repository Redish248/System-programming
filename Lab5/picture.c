#include "picture.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "/tmp/mmapped.bin"

int fd;
pixel_t* pixel;
int result;

read_status_t from_bmp(FILE * in, image_t * const in_image) {
	////mapping
	fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }
	result = lseek(fd, 3*(in_image->height)*(in_image->width) - 1, SEEK_SET);
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
	exit(EXIT_FAILURE);
    }
	result = write(fd, "", 1);
    if (result != 1) {
	close(fd);
	perror("Error writing last byte of the file");
	exit(EXIT_FAILURE);
    }
    ////mapping

	bmp_header_t bmpHeader;
	fread(&bmpHeader, sizeof(bmp_header_t), 1, in);
	if (bmpHeader.bfType != 0x4D42 || bmpHeader.biBitCount != 24) {
		return READ_INVALID_HEADER;
	}
	in_image->height = bmpHeader.biHeight;
	in_image->width = bmpHeader.biWidth;
	pixel = (pixel_t*)malloc(sizeof(pixel_t) * in_image->height * in_image->width);

	////mapping
    size_t fileSize = 3*(in_image->height)*(in_image->width);
    pixel = mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pixel == MAP_FAILED) {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
	////mapping

	//skip header
	fseek(in, bmpHeader.bOffBits, SEEK_SET);
	uint32_t i;
	for (i = 0; i < in_image->height; i++) {
		//read next string
		int a = fread(pixel + (in_image->height - i - 1) * in_image->width, sizeof(pixel_t), in_image->width, in);
		if (a != in_image->width) {
			return READ_INVALID_BITS;
		}
		//skip rubbish
		fseek(in, in_image->width % 4, SEEK_CUR);
	}
	in_image->data = pixel;
	return READ_OK;
}

image_t rotate(image_t const image) {
	image_t rotatedImage;
	uint32_t height = image.height;
	uint32_t width = image.width;
	pixel_t* pixels = (pixel_t*)malloc(sizeof(pixel_t)*width*height);
	uint32_t i, j;
	for (i = 0; i < width; ++i) {
		for (j = 0; j < height; ++j) {
			*(pixels + i*height+j) = *(image.data + j*width+width-i-1);
		}
	}
	rotatedImage.height = width;
	rotatedImage.width = height;

    ////mapping
    if (munmap(image.data,sizeof(image.data)) == -1) {
        perror("Error un-mmapping the file");
    }
    ////mapping

    rotatedImage.data = pixels;
	return rotatedImage;
}

write_status_t to_bmp(FILE* out, image_t const* image, FILE* in) {
	bmp_header_t header;

	header.bfType = 0x4D42;
	header.bfileSize = header.bOffBits + 3 * image->width * image->height + image->height * (image->width % 4);
	header.bfReserved = 0;
	header.bOffBits = sizeof(header); 
	header.biSize = 40;	
	header.biWidth = image->width;
	header.biHeight = image->height;
	header.biPlanes = 1;
	header.biBitCount = 24;
	header.biCompression = 0;
	header.biSizeImage = (image->width * 3 + 4 - image->width*3 % 4)*image->height;
	header.biXPelsPerMeter = 0;
	header.biYPelsPerMeter = 0;
	header.biClrUsed = 0;
	header.biClrImportant = 0;
	
	//writing header to file
	int r = fwrite(&header, sizeof(bmp_header_t), 1, out);
	fflush(out);
	fseek(out, header.bOffBits, SEEK_SET);
	uint32_t i;
	//writing pixels
	for (i = 0; i < image->height; i++) {
		int m = fwrite(image->data + (image->height - i - 1)*image->width, sizeof(pixel_t), image->width, out);
		if (m != image->width) {
			return WRITE_ERROR;
		}
		fflush(out);
		//skip rubbish
		fseek(out,image->width%4,SEEK_CUR);
		fflush(out);
	}

    ////mapping
    size_t size = (image->width * 3 + 4 - image->width*3 % 4)*image->height;
	if (munmap(pixel,size) == -1) {
	perror("Error un-mmapping the file");
    }
    ////mapping

    close(fd);
	return WRITE_OK;
}

