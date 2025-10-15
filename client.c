#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client.h"

#define PACKAGE_SIZE 1024
#define PIXELCOUNT (480 * 272)

void send_packets(char *file_buffer);
void build_udp_packet(char *file_buffer, char* buffer, uint16_t packet_id, uint16_t packet_amt, uint32_t offset, int payload_size, int total_bytes, uint16_t expected_packets);

int send_msg_udp(char *data, int size_data){ // sends single packet to server, returns 0 if successful, else -1
    // struct msghdr *packet = malloc(1024);
    // packet->msg_name = (struct sockaddr *)&addr;


    // // test-data to simulate input
    // char test_message[] = "testmessage";
    // int test_size = strlen(test_message);

    struct sockaddr_in servaddr;
        servaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // sends inputted data to udp server
    sendto(sockfd, data, size_data, 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    close(sockfd);

    return 0;
}

void send_packets(char *file_buffer){
    int total_bytes = 54 + (PIXELCOUNT * 3); // = 391734    pixel amount + 54 (BMP header size)
    int payload_size = 1014;    // payload 1014 since 1024 -    10 (header) = 1014
    unsigned char buffer[1024];

    int total_packets = (total_bytes + payload_size - 1) / payload_size; // rounded up since last packet is smaller than 1. total bytes + payload_size - 1 rounds total packets correctly
    printf("Expected packets:%d\n", total_packets);
    
    struct sockaddr_in servaddr;
        servaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // sends total_packets amount of packets to server
    for(int packet_id = 0; packet_id < total_packets; packet_id++){
        int offset = packet_id * payload_size;

        build_udp_packet(file_buffer, buffer, packet_id, total_packets, offset, payload_size, total_bytes, total_packets);
        
        // payload_size + 10. 10 byte + 1014 = 1024
        sendto(sockfd, buffer, (payload_size + 10), 0, (const struct sockaddr*)&servaddr, sizeof(servaddr)); 
        // free(buffer); // no longer heap buffer

        printf("Package number: %d sent\n", packet_id + 1);

        usleep(500); //sleeps   for 0,5ms
    }
    close(sockfd);
}

void build_udp_packet(char *file_buffer, char* buffer, uint16_t packet_id, uint16_t packet_amt, uint32_t offset, int payload_size, int total_bytes, uint16_t expected_packets){
    // packet_id  = packets id to help rebuild on server side
    // packet_amt = total amount of packets that will be sent
    // offset     = pixel offset in the picture
    // expected_packets = total packets to be sent
    
    if(offset + payload_size > total_bytes){ // makes sure last payload isn't longer than buf
        payload_size = total_bytes - offset;
    }

    // sets first 10 byte to contain packet information
    memcpy(&buffer[0], &packet_id, 2);          // buffer[0-1] = packet_id
    memcpy(&buffer[2], &packet_amt, 2);         // buffer[2-3] = packet_amt
    memcpy(&buffer[4], &offset, 4);             // buffer[4-7] = offset
    memcpy(&buffer[8], &expected_packets, 2);   // buffer[8-9] = expected_packets

    // buffer[10 + i] since first 10 byte contains header information
    for(int i = 0; i < payload_size; i++){ 
        buffer[10 + i] = file_buffer[offset + i];
    }
}

int wait_for_request(){ // returns 1 if got message !! unused
    char buffer[8];

    struct sockaddr_in servaddr, clientaddr;
        servaddr.sin_addr.s_addr = INADDR_ANY; // listens to all incoming
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int len = sizeof(clientaddr);
    
    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    int n = 0;
    while(!n){
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len);
    }
    
    puts((const char *)buffer);

    close(sockfd);
    return 1;
}