#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/socket.h"
#include "include/packet.h"
#include "include/file.h"
#include "include/main.h"


socketArgs_t g_socket;
firmwareArgs_t g_firmware;


int main(int argc, char **argv)
{
    char fileName[20];

    if(argc > 1) {
        strcpy(fileName, (char *)&argv[1][0]);
        load_program_file(fileName);
    }
    else {
        load_program_file(DefaultFileName);
    }


    create_socket(&g_socket);
    bind_socket(&g_socket, TARGET_PORT);



    while(1) {
        parse_packet();
//        sleep(1);
//        send_simple_packet(packet_type_header_ack);
    }

    close_socket(&g_socket);
    return 0;
}