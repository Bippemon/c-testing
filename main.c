#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>

#include "client.h"


#define PIC_WIDTH 480
#define PIC_HEIGHT 272
#define ROW_LENGTH 1440 // length of row 480 * 3(bytes per pixels)
#define PIXEL_START 54


void pic_to_pic();
unsigned char *read_bmp(char *file_to_read);
void write_bmp(unsigned char *buf, char *new_file, int new_file_len);


int main(){
    

    unsigned char *picture = read_bmp("benkis.bmp");
    send_packets(picture);
    
    return 0;
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

unsigned char *read_bmp(char *file_to_read){ 
    unsigned char *buffer;
    size_t file_len;
    
    clock_t clock_start;
    clock_t clock_end;
    
    clock_start = clock(); //starts ms clock counter
    
    FILE *png = fopen(file_to_read, "rb");
    fseek(png, 0, SEEK_END);
    file_len = ftell(png);
    rewind(png);
    
    // allocate memory on heap
    buffer = malloc(file_len * sizeof(char));
    fread(buffer, 1, file_len, png);
    fclose(png);

    return buffer; // remove return to 

    // this is ignored
    write_bmp(buffer, "new_bmp.bmp", file_len); // frees memory after done
    
    buffer = NULL; // removes dangling pointer

    clock_end = clock(); // ends ms time counter
    printf("\ntime taken: %f sec \n", (double)(clock_end - clock_start)/CLOCKS_PER_SEC); // print total time taken

    char test_msg[] = "Äter marg simpson hon smakan go'";
    const int msg_size = 64;
    char *msg = malloc(msg_size);
    strcpy(msg, test_msg);
    
    printf("Message to send: %s\n", msg);

    // // send msg 
    // if(send_msg_udp(msg, msg_size) == -1){
    //     printf("Message could NOT be sent..\n");
    // }

    free(msg);
    msg = NULL;

}

 void write_bmp(unsigned char *buf, char *new_file, int new_file_len){
    FILE *new_pic = fopen(new_file, "wb");  // opens new file
    fwrite(buf, 1, 54, new_pic);            // write header to new file
    
    /* 
    writes bytes to new file in chunks of 3 at a time.
    writes in order B, G, A
    pixel data starts on i = 54 since header contains 14+40 bytes of data
    */
    for(int i = 54; i < new_file_len; i += 3){
        fwrite(&buf[i], 1, 3, new_pic);
    }

    free(buf); // frees malloc buffer after copying
}
