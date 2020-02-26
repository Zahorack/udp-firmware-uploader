//
// Created by oholly on 26. 2. 2020.
//

#include "../include/utils.h"
#include <string.h>

#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

static HANDLE inputThread;
int g_userInputState;

int inet_pton4 (const char *src, const char *end, unsigned char *dst)
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

LPTHREAD_START_ROUTINE input()
{
    char in[512];

    while(1) {
        scanf("%s", in);

        printf("inpu: %s", in);

        g_userInputState = 1;

        printf("\nDo u want upgrade firmware? [y/n]  ");
        char answer;
        scanf("%c", &answer);
        answer = getchar();

        if(answer == 'y') {

            printf("\nFile name %s ? [y/n]   ", g_firmware.fileName);
            scanf("%c", &answer);
            answer = getchar();

            if(answer == 'n') {
                printf("Type file name: ");
                scanf("%s", &g_firmware.fileName[0]);
                load_program_file(g_firmware.fileName);
            }

            int probeId;
            printf("\nType probe index:  (from 0 to %d)", g_probeCount-1);
            scanf("%d", &probeId);
            answer = getchar();

            g_firmware.probeIndex = probeId;

            send_firmware_header(&g_firmware);
            send_firmware_data(&g_firmware);
        }
    }


}

void start_input_thread()
{
    inputThread = CreateThread(
            NULL,                                              // default security attributes
            0,                                                 // use default stack size
            (LPTHREAD_START_ROUTINE)input,                     // thread function name
            NULL,                                              // argument to thread function
            0,                                                 // use default creation flags
            NULL);                                             // returns the thread identifier
}


void stop_threads()
{
    CloseHandle(inputThread);
}