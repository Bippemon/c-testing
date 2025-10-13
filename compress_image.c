#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_BYTES 522240

typedef struct {
    FILE *fp;
    long length;
} Image;

Image open_image(const char *file_name);
Image compress_image(Image *img);
void write_image(Image *img);

int main(){
    Image img = open_image("original.bmp");

    write_image(&img);

    // unsigned char *buffer = compress_image(img.fp);
    
    // recreate_bmp(buffer);
}

Image open_image(const char *file_name){
    Image img = {0};
    img.fp = fopen(file_name, "rb");
    
    fseek(img.fp, 0, SEEK_END);
    img.length = ftell(img.fp);
    rewind(img.fp);


    return img;
}

void write_image(Image *img){
    FILE *new_pic = fopen("test.bmp", "wb");

    unsigned char buffer[1024];

    int amt_read;
    while((amt_read = fread(buffer, 1, 1024, img->fp)) > 0){
        fwrite(buffer, 1, amt_read , new_pic);
    }
    fclose(img->fp);
    fclose(new_pic);
}

// Image compress_image(Image *img){
    
//     printf("Picture size = %lu\n", sizeof(picture));
    
//     char *buffer = malloc(file_len);
//     fread(buffer, 1, file_len, picture);
//     fclose(picture);
    
//     return buffer;
// }

// void recreate_bmp(unsigned char *buffer){
//     FILE *new_pic = fopen("test.bmp", "wb");

//         fwrite(buffer, 1, file_len, new_pic);
//         fclose(new_pic);
    
    
    
//     const int buf_size = 1024;
//     unsigned char *buffer = malloc(buf_size);
    
//     buffer = realloc(buffer, buf_size * 2);

// }