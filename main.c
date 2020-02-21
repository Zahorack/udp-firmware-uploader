#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>


#include "include/socket.h"
#include "include/packet.h"
#include "include/file.h"
#include "include/main.h"
#include "include/timer.h"

//global
socketArgs_t g_socket;
firmwareArgs_t g_firmware;
probeInfo_t g_probe;
senderSates_t g_senderState;

//static
timerArgs_t s_timer;
static void sender();


#define NS_INADDRSZ        4
static int inet_pton4 (const char *src, const char *end, unsigned char *dst)
{
    int saw_digit, octets, ch;
    unsigned char tmp[NS_INADDRSZ], *tp;
    saw_digit = 0;
    octets = 0;
    *(tp = tmp) = 0;
    while (src < end)
    {
        ch = *src++;
        if (ch >= '0' && ch <= '9')
        {
            unsigned int new = *tp * 10 + (ch - '0');
            if (saw_digit && *tp == 0)
                return 0;
            if (new > 255)
                return 0;
            *tp = new;
            if (! saw_digit)
            {
                if (++octets > 4)
                    return 0;
                saw_digit = 1;
            }
        }
        else if (ch == '.' && saw_digit)
        {
            if (octets == 4)
                return 0;
            *++tp = 0;
            saw_digit = 0;
        }
        else
            return 0;
    }
    if (octets < 4)
        return 0;
    memcpy (dst, tmp, NS_INADDRSZ);
    return 1;
}

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

//    char ip[50];
//    printf("Type IP ADDRES\n");
//    scanf("%s", ip);
//
//    printf("IP: %s\n",  ip);
//
//    int len = strlen(ip);
//    inet_pton4(&ip[0], &ip[0] + len, &g_socket.target_address.sin_addr);
//
//    printf("g_socket.target_address.sin_addr: %s",  g_socket.target_address.sin_addr);


    create_socket(&g_socket);
    bind_socket(&g_socket, TARGET_PORT);
//    listen(g_socket.socket, 5);

    while(1) {
        parse_packet();

        if(g_firmware.state == program_is_sending) {
            sender();
        }
    }

    close_socket(&g_socket);
    return 0;
}

static void sender()
{
    static uint16_t send_tries = 0;
    static uint16_t duplication_sent = 0;

    if(g_senderState != sender_state_sent){
        send_tries = 0;
        send_firmware_data(&g_firmware);
        timer_start(&s_timer, 1);
        //sent_time = HAL_GetTick();
    }
    else{
        if(timer_run(&s_timer)){
            duplication_sent++;
            send_tries++;
            printf("timeout \r\n");
            send_firmware_data(&g_firmware);

            if(send_tries > 3){
                //device_state = device_disconnected;
                g_senderState = sender_state_none;
                printf("MAX nglt frame resend tries reached...\r\n");
            }
        }
    }
}