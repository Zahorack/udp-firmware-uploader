//
// Created by oholly on 6. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_FILE_H
#define UDP_FIRMWARE_UPLOADER_FILE_H

#include <stdio.h>
#include <stdlib.h>

extern char DefaultFileName[];

extern char program[100000]; //100k max


extern void load_program_file(char *name);

#endif //UDP_FIRMWARE_UPLOADER_FILE_H
