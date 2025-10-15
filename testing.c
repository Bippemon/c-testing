#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){

    FILE *file = fopen("test.txt", "rb");

    char buffer[50];
    fread(buffer, sizeof(file), 50, file);

    for (int i = 0; i < 50; i++){
        printf("%c", buffer[i]);
    }

}