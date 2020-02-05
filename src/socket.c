//
// Created by oholly on 5. 2. 2020.
//

#include "../include/socket.h"




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
    arg->source_address.sin_family = AF_INET;
    arg->source_address.sin_port = htons(port);
    arg->source_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(arg->socket, (struct sockaddr *) &arg->source_address, sizeof(arg->source_address)) == SOCKET_ERROR)
    {
        printf("Error\nCall to bind(s, (struct sockaddr *) &addr, sizeof(addr)); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }
    printf("OK\n");

    // Receive, to store target address
    char data[512];
    receive_socket(&g_socket, data, 512);
}

int receive_socket(socketArgs_t *arg, char *data, int size)
{
    int targetAddrLen = sizeof(arg->target_address);

    int ret = recvfrom(arg->socket, data, size, 0, (struct sockaddr *)&arg->target_address, &targetAddrLen);


    if (ret == SOCKET_ERROR) {
        printf("Error\nCall to recvfrom(s, data, dataLen, 0, (struct sockaddr *) &remote_addr, &iRemoteAddrLen); failed with:\n%d\n",
        WSAGetLastError());
        exit(1);
    }

    int dataLen = ret;        // Length of the data received
    data[dataLen] = '\0';        // Convert to cstring
    for (int i = 0; i < ret; i++)
    {
        printf("%02X.", data[i]);
    }
    printf(" received from %s, %d, size: %d\n", inet_ntoa(arg->target_address.sin_addr), (int)ntohs(arg->target_address.sin_port),  ret);
}

void send_socket(socketArgs_t *arg, char *data, int size)
{
    int targetAddrLen = sizeof(arg->target_address);
    sendto(arg->socket, data, size, 0, (struct sockaddr *) &arg->target_address, &targetAddrLen);
}

void close_socket(socketArgs_t *arg)
{
    closesocket(arg->socket);

    WSACleanup();
}