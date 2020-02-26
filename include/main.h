//
// Created by oholly on 5. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_MAIN_H
#define UDP_FIRMWARE_UPLOADER_MAIN_H

#include <stdint.h>

enum ProgramStateEnum {
    program_available = 0,
    program_is_sending,
    program_was_send
};

typedef struct {
    char *bin;
    uint32_t size;
    uint16_t block_index;
    char fileName[20];
    enum ProgramStateEnum state;
    int probeIndex;
}firmwareArgs_t;

extern firmwareArgs_t g_firmware;


typedef enum{
    sender_state_none= 0,
    sender_state_sent,
    sender_state_confirmed
}senderSates_t;

extern senderSates_t g_senderState;

#endif //UDP_FIRMWARE_UPLOADER_MAIN_H
