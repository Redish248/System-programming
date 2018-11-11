#include <stdlib.h>
#include <stdio.h>
#include "picture.h"

int main() {
	FILE* image;
	FILE* newImage;
	char filename[256];
	read_status_t read_code;
	write_status_t write_code;
	image_t in_image;
	image_t out_image;

	printf("Enter file name:\n");
	scanf("%s",filename);

	image = fopen(filename, "rb");
	if (image == NULL) {
		printf("This file doesn't exist!\n");
	}
	else {
		read_code = from_bmp(image, &in_image);
		if (read_code == READ_OK) {
			out_image = rotate(in_image);
			newImage = fopen("newImage.bmp", "wb");
			write_code = to_bmp(newImage, &out_image,image);
			switch (write_code) {
			case WRITE_OK:
				printf("Created new rotated image");
				break;
			default:
				printf("Some errors when image created");
				break;
			}
			fflush(newImage);
			fclose(newImage);
		} else{
			switch (read_code) {
			case READ_INVALID_SIGNATURE:
				printf("invalid signature!\n");
				break;
			case READ_INVALID_BITS:
				printf("invalid bits!");
				break;
			default:
				printf("invalid header!");
				break;
			}
		
		}
		fclose(image);
		free(out_image.data);
	}

	return 0;
}