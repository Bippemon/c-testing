#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7777
#define SERVER_ADDR "127.0.0.1" // loopback
// #define SERVER_ADDR "192.168.0.102" //local net


int server_loop(FILE *data);
void request_image();
// void write_to_file(char *buffer);

int main(){
    request_image();

    FILE *new_file = fopen("recieved-file.bmp", "wb");

    server_loop(new_file);
    
    return 0;
}

int server_loop(FILE *file_to_write){
    char buffer[1024];
    struct sockaddr_in servaddr, clientaddr;
        servaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int len = sizeof(clientaddr);
    
    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    int expected_packets = 386;

    // loops while client still sending data
    int is_recieving = 1;
    while(is_recieving){
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len);

        uint16_t packet_id, packet_amt;
        uint32_t offset;
        memcpy(&packet_id, &buffer[0], 2);  // reads bytes 0-1 to packet_id
        memcpy(&packet_amt, &buffer[2], 2); // reads bytes 2-3 to packet_amt
        memcpy(&offset, &buffer[4], 4);     // reads bytes 4-7 to offset

        // buffer + 8 = dont write metadata
        // n - 8 = to compensate for not reading first 8 bytes
        if(n >= 8){ // only write if packet larger than 8 bytes
            fseek(file_to_write, offset, SEEK_SET);
            fwrite(buffer + 8, 1, n - 8, file_to_write);
            
            expected_packets--;
        }

        // end loop if last package recieved
        if(packet_id == packet_amt - 1){
            is_recieving = 0;
            printf("Packets lost: %d\n", expected_packets);
        }
    }

    return 0;
}

void request_image(){
    char buffer[8] = "pic pls";

    struct sockaddr_in servaddr, clientaddr;
        servaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
        servaddr.sin_port = htons(PORT);
        servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sendto(sockfd, buffer, 8, 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    close(sockfd);
}

// void write_to_file(char *buffer){
    
// }