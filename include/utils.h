//
// Created by oholly on 26. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_UTILS_H
#define UDP_FIRMWARE_UPLOADER_UTILS_H


#define NS_INADDRSZ        4

#include "packet.h"
#include "file.h"

extern int g_userInputState;


extern int inet_pton4 (const char *src, const char *end, unsigned char *dst);

extern void start_input_thread();
extern void stop_threads();

#endif //UDP_FIRMWARE_UPLOADER_UTILS_H
