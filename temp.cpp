#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, int **argv)
{
    WSADATA wsda;		// Structure to store info
    // returned from WSAStartup

    char data[512];
    int dataLen = 512;
    int ret;

    char szAddress[64];
    int iPort;

    SOCKET s;					// Our TCP socket handle
    SOCKADDR_IN addr,			// The local interface address
            remote_addr;		// The sender's address
    int iRemoteAddrLen;			// Contains the length of remte_addr, passed to recvfrom

    // Check arguments
    if(argc != 2 ||
       (argc==2 && strcmp((char *) &argv[1][0], "/?")==0))
    {
        printf("wsudprcv port\n");
        printf("   port:   the port the server should listen to\n");
        exit(1);
    }

    //iPort = atoi((char *) &argv[1][0]);
    iPort = 54321;

    WSAStartup(MAKEWORD(1,1), &wsda);

    // Create an UDP socket
    printf("Creating socket...");
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Error?
    if(s == SOCKET_ERROR)
    {
        printf("Error\nCall to socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }
    printf("OK\n");

    // Fill in the interface information
    printf("Binding socket...");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(iPort);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR)
    {
        printf("Error\nCall to bind(s, (struct sockaddr *) &addr, sizeof(addr)); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }
    printf("OK\n");
    sleep(1);


    iRemoteAddrLen = sizeof(remote_addr);
    ret = recvfrom(s, data, dataLen, 0, (struct sockaddr *)&remote_addr, &iRemoteAddrLen);
    printf(" received from %s, %d, size: %d\n", inet_ntoa(remote_addr.sin_addr), (int)ntohs(remote_addr.sin_port),  ret);

    //printf(" source from %s, %d, size: %d\n", inet_ntoa(addr.sin_addr), (int)ntohs(addr.sin_port),  ret);

    // Ready to receive data
    while(1) {


//        iRemoteAddrLen = sizeof(remote_addr);
//
//        ret = recvfrom(s, data, dataLen, 0, (struct sockaddr *)&remote_addr, &iRemoteAddrLen);
//        //ret = recv(s, data, 512, 0);
//        if (ret == SOCKET_ERROR) {
//            printf("Error\nCall to recvfrom(s, data, dataLen, 0, (struct sockaddr *) &remote_addr, &iRemoteAddrLen); failed with:\n%d\n",
//                   WSAGetLastError());
//            exit(1);
//        }
//        dataLen = ret;        // Length of the data received
//        data[dataLen] = '\0';        // Convert to cstring
//        for (int i = 0; i < ret; i++)
//        {
//            printf("%02X.", data[i]);
//        }
//        printf(" received from %s, %d, size: %d\n", inet_ntoa(remote_addr.sin_addr), (int)ntohs(remote_addr.sin_port),  ret);


        sleep(1);
        char sendData[10] = {0,1,2,3,4,5,6,7,8,9};

        sendto(s, sendData, 10, 0, (struct sockaddr *) &remote_addr, &iRemoteAddrLen);
        //send(s, sendData, 5, 0);
    }
    closesocket(s);

    WSACleanup();

    return 0;
}