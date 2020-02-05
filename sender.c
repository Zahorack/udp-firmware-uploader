// author:			frenchwhale (http://frenchwhales_site.tripod.com)
//					for the WinSock tutorial
//
// file name:		wsudpsnd.c
//
// description:     A UDP packet sender that sends a
//                  UDP packet containing a pre-specified
//                  message.
// Copyright 2002 frenchwhale

#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE		"This is an UDP message."

int main(int argc, int **argv)
{
    WSADATA wsda;		// Structure to store info
    // returned from WSAStartup

    struct hostent *host;	// Used to store information
    // retreived about the server

    char szMessage[80];
    int iMessageLen;
    int ret;

    char szAddress[64];
    int iPort;

    SOCKET s;		// Our TCP socket handle
    SOCKADDR_IN addr;	// The host's address

    // Check arguments
    if(argc != 3 ||
       (argc==2 && strcmp((char *) &argv[1][0], "/?")==0))
    {
        printf("wsudpsnd server port\n");
        printf("   server: the server to send the packet to\n");
        printf("   port:   the port on the remote server\n");
        exit(1);
    }

    // Copy the IP address
    strcpy(szAddress, (char *) &argv[1][0]);

    // Get the remote port
    iPort = atoi((char *) &argv[2][0]);

    if(iPort<0 || iPort>65563)
    {
        printf("Invalid port number! (%s)\n", argv[2]);
        exit(1);
    }

    // Copy the pre-defined message into a buffer
    strcpy(szMessage, MESSAGE);

    iMessageLen = strlen(szMessage);

    // Load version 1.1 of Winsock

    WSAStartup(MAKEWORD(1,1), &wsda);

    // Create a UDP socket
    printf("Creating socket...");
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Error?
    if(s == SOCKET_ERROR)
    {
        printf("Error\nCall to socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }

    printf("OK\n");

    // Fill in the host information
    addr.sin_family = AF_INET;
    addr.sin_port = htons(iPort);
    addr.sin_addr.s_addr = inet_addr(szAddress);

    if(addr.sin_addr.s_addr == INADDR_NONE) 	// The address wasn't in numeric
        // form, resolve it
    {
        host = NULL;
        printf("Resolving host...");
        host = gethostbyname(szAddress);	// Get the IP address of the server
        // and store it in host
        if(host == NULL)
        {
            printf("Error\nUnknown host: %s\n", szAddress);
            exit(1);
        }
        memcpy(&addr.sin_addr, host->h_addr_list[0],
               host->h_length);
        printf("OK\n");
    }

    // Ready to send data

    printf("Sending packet...");
    ret = sendto(s, szMessage, iMessageLen, 0, (struct sockaddr *) &addr, sizeof(addr));

    if(ret == SOCKET_ERROR)
    {
        printf("Error\nCall to sendto(s, szMessage, iMessageLen, 0, (SOCKADDR_IN *) &addr, sizeof(addr)); failed with:\n%d\n", WSAGetLastError());
        exit(1);
    }
    printf("OK\n");

    printf("\"%s\" sent to %s\n", szMessage, szAddress);

    closesocket(s);

    WSACleanup();

    return 0;
}