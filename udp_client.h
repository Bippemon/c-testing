#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define LAPTOP_ADDR "192.168.0.220" //local net
#define DESKTOP_ADDR "192.168.0.104"

int send_msg_udp(char *data, int size_data);
int send_packets(char *buf);
char *build_udp_packet(char *buf, u_int16_t packet_id, u_int16_t packet_amt, u_int32_t offset, int payload_size, int total_bytes);
int wait_for_request();

#endif //UDP_CLIENT_H