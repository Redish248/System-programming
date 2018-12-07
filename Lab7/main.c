#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdint.h>
#include "picture.h"
#include "image_sepia.h"

int main() {
    FILE* image;
    FILE* newImage;
    FILE* sseImage;
    char filename[256];
    read_status_t read_code;
    write_status_t write_code;
    image_t in_image;
    double start, end, time_c, time_asm;

    printf("Enter file name:\n");
    scanf("%s",filename);

    image = fopen(filename, "rb");
    if (image == NULL) {
        printf("This file doesn't exist!\n");
    }
    else {
        read_code = from_bmp(image, &in_image);
        if (read_code == READ_OK) {
            struct rusage r;
            struct timeval start;
            struct timeval end;

            sepia_c_inplace(&in_image);

            getrusage(RUSAGE_SELF, &r);
            start = r.ru_utime;
            sepia_c_inplace(&in_image);
            getrusage(RUSAGE_SELF, &r);
            end = r.ru_utime;
            long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
            printf( "Time for C algorithm: %ld\n", res );


            getrusage(RUSAGE_SELF, &r);
            start = r.ru_utime;
            image_sepia_sse(&in_image);
            getrusage(RUSAGE_SELF, &r);
            end = r.ru_utime;
            res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
            printf( "Time for SSE algorithm: %ld\n", res );

            sseImage = fopen("sseImage.bmp","wb");
            write_code = to_bmp(sseImage, &in_image, image);
            newImage = fopen("newImage_c.bmp","wb");
            write_code = to_bmp(newImage, &in_image, image);
            switch(write_code) {
                case WRITE_OK:
                    printf("Created new image with sepia filter!\n");
                    break;
                case WRITE_ERROR:
                    printf("New image didn't created :(\n");
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
                    printf("invalid bits!\n");
                    break;
                default:
                    printf("invalid header!\n");
                    break;
            }

        }
        fclose(image);
        free(image);
    }

    return 0;
}