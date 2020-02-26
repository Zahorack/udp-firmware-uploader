//
// Created by oholly on 5. 2. 2020.
//

#include "../include/socket.h"
#include "../include/packet.h"


void create_socket(socketArgs_t *arg)
{
    WSAStartup(MAKEWORD(1,1), &g_wsda);
    printf("Creating socket...");

    arg->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(arg->socket == SOCKET_ERROR)
    {
        printf("Error\nCall to socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }
    printf("OK\n");
}

void bind_socket(socketArgs_t *arg, int port)
{
    printf("Binding socket...");
    arg->target_address.sin_family = AF_INET;
    arg->target_address.sin_port = htons(port);
    arg->target_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(arg->socket, (struct sockaddr *) &arg->target_address, sizeof(arg->target_address)) == SOCKET_ERROR)
    {
        printf("Error\nCall to bind(s, (struct sockaddr *) &addr, sizeof(addr)); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }
    printf("OK\n");

    arg->connection_state = 0;

    // Receive, to store target address
    char data[512];
    receive_socket(arg, data, 512);


//    printf("IP: %s\n", inet_ntoa(arg->source_address.sin_addr));
//    printf("IP: %s\n", inet_ntoa(arg->target_address.sin_addr));

}

int receive_socket(socketArgs_t *arg, char *data, int size)
{
    int targetAddrLen = sizeof(arg->target_address);

    int ret = recvfrom(arg->socket, data, size, 0, (struct sockaddr *)&arg->target_address, &targetAddrLen);


    if (ret == SOCKET_ERROR) {
        //printf("Error\nCall to recvfrom(s, data, dataLen, 0, (struct sockaddr *) &remote_addr, &iRemoteAddrLen); failed with:\n%d\n",
        //WSAGetLastError());
        //exit(1);
    }

    int dataLen = ret;        // Length of the data received
    data[dataLen] = '\0';        // Convert to cstring
}

void send_socket(socketArgs_t *arg, char *data, int size)
{
    int targetAddrLen = sizeof(arg->target_address);
    sendto(arg->socket, data, size, 0, (struct sockaddr *) &arg->target_address, targetAddrLen);
}

void close_socket(socketArgs_t *arg)
{
    closesocket(arg->socket);

    WSACleanup();
}