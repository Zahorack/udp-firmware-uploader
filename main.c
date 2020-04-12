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
#include "include/utils.h"

//global
socketArgs_t g_socket;
firmwareArgs_t g_firmware;
senderSates_t g_senderState;

//static
timerArgs_t s_timer;
timerArgs_t s_debugTimer;
static void sender();


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

    timer_start(&s_debugTimer, 2);
    start_input_thread();

    while(1) {
        parse_packet();

        if(g_firmware.state == program_is_sending) {
            sender();
        }

        if(timer_run(&s_debugTimer) && !g_userInputState) {
            print_probe_list();
        }
    }

    stop_threads();
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