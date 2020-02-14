//
// Created by oholly on 5. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_SOCKET_H
#define UDP_FIRMWARE_UPLOADER_SOCKET_H

#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_BLOCK_SIZE 100

#define TARGET_PORT 54321

typedef struct {
    SOCKET socket;
    SOCKADDR_IN source_address;
    SOCKADDR_IN target_address;
    uint8_t connection_state;

} socketArgs_t;

extern socketArgs_t g_socket;
WSADATA g_wsda;

extern void bind_socket(socketArgs_t *arg, int port);
extern void create_socket(socketArgs_t *arg);
extern int receive_socket(socketArgs_t *arg, char *data, int size);
extern void send_socket(socketArgs_t *arg, char *data, int size);
extern void close_socket(socketArgs_t *arg);

#endif //UDP_FIRMWARE_UPLOADER_SOCKET_H
