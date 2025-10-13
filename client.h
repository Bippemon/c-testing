#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdint.h>

#define PORT 7777

#define SERVER_ADDR "127.0.0.1" 
#define CLIENT_ADDR "127.0.0.1" 

int send_msg_udp(char *data, int size_data);
void send_packets(char *file_buffer);
void build_udp_packet(char *file_buffer, char* buffer, uint16_t packet_id, uint16_t packet_amt, uint32_t offset, int payload_size, int total_bytes, uint16_t expected_packets);
int wait_for_request();

#endif //UDP_CLIENT_H