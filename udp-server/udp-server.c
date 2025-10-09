#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7777


int main(){

    char buffer[1024];
    struct sockaddr_in servaddr, clientaddr;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    int len = sizeof(clientaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len);
    buffer[n] = '\0';
    puts(buffer);


    return 0;
}