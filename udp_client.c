#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "udp_client.h"

#define PORT 7777
#define PACKAGE_SIZE 1024
#define PIXELCOUNT 130560

int send_packets(char *buf);
char *build_udp_packet(char *buf, uint16_t packet_id, uint16_t packet_amt, uint32_t offset, int payload_size, int total_bytes);

int send_msg_udp(char *data, int size_data){ // sends single packet to server, returns 0 if successful, else -1
    // struct msghdr *packet = malloc(1024);
    // packet->msg_name = (struct sockaddr *)&addr;


    // // test-data to simulate input
    // char test_message[] = "testmessage";
    // int test_size = strlen(test_message);

    struct sockaddr_in servaddr;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // sends inputted data to udp server
    sendto(sockfd, data, size_data, 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    close(sockfd);

    return 0;
}

int send_packets(char *buf){
    int total_bytes = 54 + (PIXELCOUNT * 3); // = 391734
    int payload_size = 1016;    // payload 1016 since 1024 - 8 (header) = 1016
    int total_packets = 386;    // totalbytes (391734) / payload size (1016) ~= 386    385,5 rounded up
    
    struct sockaddr_in servaddr;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    for(int packet_id = 0; packet_id < total_packets; packet_id++){
        int offset = packet_id * payload_size;

        char *buffer = build_udp_packet(buf, packet_id, total_packets, offset, payload_size, total_bytes);
        
        // payload_size + 8. 8 byte + 1016 = 1024
        sendto(sockfd, buffer, (payload_size + 8), 0, (const struct sockaddr*)&servaddr, sizeof(servaddr)); 
        free(buffer);

        printf("Package number: %d\n", packet_id);

        usleep(500); //sleeps for 1ms
    }
    close(sockfd);
    return 0;
}

char *build_udp_packet(char *buf, uint16_t packet_id, uint16_t packet_amt, uint32_t offset, int payload_size, int total_bytes){
    // packet_id  = packets id to help rebuild on server side
    // packet_amt = total amount of packets that will be sent
    // offset     = pixel offset in the picture

    char *buffer = malloc(1024);// data to send
    if(!buffer){ // if allocation failed
        return NULL;
    }
    
    if(offset + payload_size > total_bytes){ // makes sure last payload isn't longer than buf
        payload_size = total_bytes - offset;
    }

    memcpy(&buffer[0], &packet_id, 2);  // buffer[0-1] = packet_id
    memcpy(&buffer[2], &packet_amt, 2); // buffer[2-3] = packet_amt
    memcpy(&buffer[4], &offset, 4);     // buffer[4-7] = offset

    for(int i = 0; i < payload_size; i++){
        buffer[8 + i] = buf[offset + i];
    }

    return buffer;
}