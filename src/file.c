//
// Created by oholly on 6. 2. 2020.
//

#include <mem.h>
#include "../include/file.h"
#include "../include/main.h"

char DefaultFileName[] = "LTD_STM32F407VET6.bin";
char program[100000];

static FILE *fptr;


void load_program_file(char *name)
{
    if ((fptr = fopen(name, "rb")) == NULL) {
        printf("Error! opening bin file");
        exit(1);
    }

    char c;
    int iterator = 0;

    while (!feof(fptr)){
        program[iterator] = fgetc(fptr);
        iterator++;
    }

    g_firmware.size = iterator;
    g_firmware.bin = program;
    g_firmware.state = program_available;

    strcpy(g_firmware.fileName, name);

    printf("Data from the file [%d]:\n%s",g_firmware.size, program);
    fclose(fptr);
}
