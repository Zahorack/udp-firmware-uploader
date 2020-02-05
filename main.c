#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/socket.h"
#include "include/packet.h"

socketArgs_t g_socket;

int main(int argc, int **argv)
{

    create_socket(&g_socket);
    bind_socket(&g_socket, TARGET_PORT);


    while(1) {
        parse_packet();
    }

    return 0;
}