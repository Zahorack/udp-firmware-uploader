//
// Created by oholly on 5. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_MAIN_H
#define UDP_FIRMWARE_UPLOADER_MAIN_H

enum ProgramStateEnum {
    program_available = 0,
    program_is_sending,
    program_was_send
};

typedef struct {
    char *bin;
    int size;
    int block_index;
    enum ProgramStateEnum state;
}firmwareArgs_t;

extern firmwareArgs_t g_firmware;

#endif //UDP_FIRMWARE_UPLOADER_MAIN_H
