#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define TOTAL_BYTES_RGBA 522240
#define TOTAL_BYTES 391680
#define PACKET_SIZE 1024
#define BMP_HEADER 54

typedef struct {
    unsigned char *data;
    long length;
} Image;

Image open_image(const char *file_name);
Image compress_image(const char *file_name);
void write_image(Image *img);

int main(){
    Image img = compress_image("original.bmp");
}

Image open_image(const char *file_name){ // returns a finished struct
    
    FILE *fp = fopen(file_name, "rb");
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    unsigned char *buffer = malloc(file_size);
    fread(buffer, 1, file_size, fp);
    fclose(fp);

    Image img = {buffer, file_size};

    return img;
}

void write_image(Image *img){
    FILE *new_pic = fopen("test.bmp", "wb");

    int packets = 0, totalsize = 0;

    fwrite(img->data, 1, img->length, new_pic);

    fclose(new_pic);
}

Image compress_image(const char *file_name){
    FILE *fp = fopen(file_name, "rb");
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    unsigned char *buf = malloc(file_size);
    fread(buf, 1, file_size, fp);
    fclose(fp);

    Image img = {buf, file_size};

    unsigned char *compressed = malloc(img.length);

    // 1 packet
    const int buffer_size = 1024;
    unsigned char buffer[buffer_size];
    int offset = 0;
    int filled_buffers = 0;

    /* [  B  ][  G  ][  R  ][px_count] */    

    unsigned char last_px[3], current[3];
    unsigned char same_pixel_count = 0;

    for(int i = BMP_HEADER; i < TOTAL_BYTES + BMP_HEADER; i += 3){
        // set current pixel values
        memcpy(current, img.data + i, 3); // copies 3 bytes of data to "current" buffer
        
        
        // sets first occurence of px value and continues
        if (same_pixel_count == 0){ // new pixel
            // printf("test2\n");
            memcpy(last_px, current, 3); // copies current px values to last_px
            same_pixel_count = 1;
        }
        // if last and current byte do not match or reach buffer limit, adds last_px to buffer
        if (memcmp(&last_px, &current, 3) != 0 || same_pixel_count == 255){
            // printf("test3\n");
            memcpy(&buffer[offset], last_px, 3);               // adds color data
            memcpy(&buffer[offset + 3], &same_pixel_count, 1);  // adds px amount
            offset += 4; 
            
            // adds to compressed when buffer filled
            if (offset == buffer_size){ // if 1024 bytes has been added
                memcpy(&compressed[filled_buffers * buffer_size], buffer, buffer_size); // copies full buffer to compressed
                filled_buffers++;
                
                // start from begingin
                offset = 0;
            }
            
            same_pixel_count = 0;
        }   
        if(memcmp(last_px, current, 3) == 0){
            // if not first pixel and same as last
            same_pixel_count++;
        }
    } // end for loop
    if(offset > 0){ //saker kvar
        printf("bytes left in buffer: %d\n", offset);

        memcpy(&compressed[filled_buffers * buffer_size], buffer, offset);
        filled_buffers++;
    }

    
    int new_buffer_size = (filled_buffers - 1) * buffer_size + offset;

    compressed = realloc(compressed, new_buffer_size);

    Image compressed_img = {compressed, new_buffer_size};
    
    printf("filled buffers: %d\n", filled_buffers);
    printf("offset: %d\n", offset);
    printf("size of compressed img: %u\n", new_buffer_size);
    
    return compressed_img;

    // Image compressed_img = {compressed, };
    

    // return compressed_img;
}

// void recreate_bmp(unsigned char *buffer){
//     FILE *new_pic = fopen("test.bmp", "wb");

//         fwrite(buffer, 1, file_len, new_pic);
//         fclose(new_pic);
    
    
    
//     const int buf_size = 1024;
//     unsigned char *buffer = malloc(buf_size);
    
//     buffer = realloc(buffer, buf_size * 2);

// }