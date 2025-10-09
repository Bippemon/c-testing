#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define PIXELCOUNT 130560
#define PIC_WIDTH 480
#define PIC_HEIGHT 272
#define ROW_LENGTH 1440 // length of row 480 * 3(bytes per pixels)
#define PIXEL_START 54


void pic_to_pic();
void read_bmp();

int main(){
    // pic_to_pic();

    read_bmp();
}

void pic_to_pic(){
// read first img file to heap
    FILE *file = fopen("original.png", "rb"); 

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    void *data = malloc(file_size);
    memset(data, 0, file_size);
    
    fread(data, 1, file_size, file);
    fclose(file);


    // read first img file and save to file
    FILE *text = fopen("text.txt", "wb");
    fwrite(data, 1, file_size, text);
    fclose(text);
    
    
    // read from text file to heap
    FILE *old_file = fopen("text.txt", "rb");
    fseek(old_file, 0, SEEK_END);
    file_size = ftell(old_file);
    fseek(old_file, 0, SEEK_SET);

    free(data);
    data = malloc(file_size);
    memset(data, 0, file_size);

    fread(data, 1, file_size, old_file);
    fclose(old_file);


    // save data to new png file
    FILE *new_png = fopen("new_png.png", "wb");
    fwrite(data, 1, file_size, new_png);
    fclose(new_png);

    free(data);
}

void read_bmp(){
    unsigned char *buffer;
    size_t file_len;
    
    clock_t clock_start;
    clock_t clock_end;
    
    clock_start = clock(); //starts ms clock counter
    
    FILE *png = fopen("original.bmp", "rb");
    fseek(png, 0, SEEK_END);
    file_len = ftell(png);
    rewind(png);
    
    // allocate memory on heap
    buffer = malloc(file_len * sizeof(char));
    fread(buffer, 1, file_len, png);
    fclose(png);

    const int counted_pixels = (file_len - PIXEL_START) / 3;

    printf("Expected pixels: %d\ncounted pixels: %d\n", PIXELCOUNT, counted_pixels);
    printf("buffer size: %ld\n", (file_len - 54) /3);

    FILE *new_pic = fopen("new_bmp.bmp", "wb");
    fwrite(buffer, 1, 54, new_pic);


    for(int i = 54; i < file_len; i += 3){
        fwrite(&buffer[i], 1, 3, new_pic); // writes 3 byte at a time in order: B, G, A
    }

    free(buffer); // frees malloc buffer

    clock_end = clock(); // ends ms time counter

    printf("\ntime taken: %f ms\n", (double)(clock_end - clock_start)); // print total time taken
}
