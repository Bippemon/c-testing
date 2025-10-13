#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define PORT 7777

#define LAPTOP_ADDR "192.168.0.220" 
#define DESKTOP_ADDR "172.27.134.247" // WSL ip. Changes on system restart

int send_msg_udp(char *data, int size_data);
int send_packets(char *buf);
char *build_udp_packet(char *buf, uint16_t packet_id, uint16_t packet_amt, uint32_t offset, int payload_size, int total_bytes);
int wait_for_request();

#endif //UDP_CLIENT_H