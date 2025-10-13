#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../client.h"


int server_loop(FILE *data);
void request_image();
// void write_to_file(char *buffer);

int main(){

    FILE *new_file = fopen("recieved-file.bmp", "wb");
    printf("Creating new file for BMP\nWaiting for data\n");

    server_loop(new_file);
    
    return 0;
}

int server_loop(FILE *file_to_write){ // recieves packets and rebuilds bmp file
    char buffer[1024];
    struct sockaddr_in servaddr, clientaddr;
        servaddr.sin_addr.s_addr = INADDR_ANY; // listens to any incoming
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int len = sizeof(clientaddr);
    int packets_recieved = 0;
    
    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));


    // loops while client still sending data
    int is_recieving = 1;
    while(is_recieving){
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len);

        // add expected_packets to packet
        uint16_t packet_id, packet_amt, expected_packets;
        uint32_t offset;
        memcpy(&packet_id, &buffer[0], 2);          // reads bytes 0-1 to packet_id
        memcpy(&packet_amt, &buffer[2], 2);         // reads bytes 2-3 to packet_amt
        memcpy(&offset, &buffer[4], 4);             // reads bytes 4-7 to offset
        memcpy(&expected_packets, &buffer[8], 2);   // reads bytes 8-9 to expected_packets


        // buffer + 10 = dont write metadata
        // n - 10 = to compensate for not reading first 10 bytes
        if(n >= 10){ // only write if packet larger than 10 bytes
            fseek(file_to_write, offset, SEEK_SET);
            fwrite(buffer + 10, 1, n - 10, file_to_write);
            
            packets_recieved++;
        }

        // end loop if last package recieved
        if(packet_id == packet_amt - 1){
            is_recieving = 0;
            printf("Packets lost: %d\n", (expected_packets - packets_recieved));
        }
    }
    fclose(file_to_write);
    return 0;
}

void request_image(){ // unused
    printf("Requesting from address: %s\n", SERVER_ADDR);

    char buffer[8] = "pic pls";

    struct sockaddr_in servaddr, clientaddr;
        servaddr.sin_addr.s_addr = inet_addr(CLIENT_ADDR);
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // binds port to recieve connections
    if (bind(sockfd, (const struct sockaddr*) &servaddr, sizeof(servaddr)) != 0){
        printf("Socket bind failed\n");
        exit(0);
    }
    else{
        printf("Socket bind successful\n");
    }

    // starts listening on socket
    if (listen(sockfd, 1) != 0){
        printf("Listen failed\n");
        exit(0);
    }
    else{
        printf("Listening...\n"); 
    }

    int len = sizeof(clientaddr);

    // accepts incoming communication
    int connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
    if (connfd < 0){
        printf("Server accept failed\n");
        exit(0);
    }
    else {
        printf("Server accept successful\n");
    }

    send(connfd, buffer, 8, 0);

    //sendto(sockfd, buffer, 8, 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    
    close(sockfd);
    close(connfd);
}

// void write_to_file(char *buffer){
    
// }