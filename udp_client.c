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

int send_msg_udp(char *data, int size_data){ // returns 0 if successful, else -1
    // struct msghdr *packet = malloc(1024);
    // packet->msg_name = (struct sockaddr *)&addr;

    // declare test-data to simulate input
    char test_message[] = "testmessage";
    int test_size = strlen(test_message);
    printf("size of test: %d\n", test_size);


    struct sockaddr_in servaddr;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(connect(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("\nError: Connection failed!\n");
            return -1;
    }

    // sends message to udp server
    sendto(sockfd, test_message, PACKAGE_SIZE, 0, (const struct sockaddr*)NULL, sizeof(servaddr));

    close(sockfd);
    printf("Message sent!\n");

    return 0;
}

int main(){

    send_msg_udp(NULL, 0);
}