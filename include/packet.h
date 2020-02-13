//
// Created by oholly on 5. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_PACKET_H
#define UDP_FIRMWARE_UPLOADER_PACKET_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

enum packet_type {
    packet_type_none_server =0,
    packet_type_status,
    packet_type_NGLT_config,
    packet_type_NGLT_meassure,
    packet_type_NGLT_data,
    packet_type_LED_config,
    packet_type_LED_meassure,
    packet_type_NanEye_config,
    packet_type_NanEye_meassure,
    packet_type_NanEye_data,
    packet_type_bussy,
    packet_type_error,
    packet_type_ack,
    packet_type_data_ack,
    packet_type_header_ack,
    packet_type_nack,
    packet_type_IMU_data,
    packet_type_TEMP_data,
    packet_type_System_reset,
    packet_type_PoE_meassure,
    packet_type_flash_erase,
    packet_type_capability_test,
    packet_type_firmware_index,
    packet_type_custom_trigger,
    packet_type_read_davis_ID,
    packet_type_write_davis_ID,
    packet_type_clear_davis_ID,
    packet_type_bootloader,
    packet_type_firmware_data,
    packet_type_firmware_header
};

#pragma pack(push,1)
typedef struct {
    uint32_t	probeID_1;
    uint32_t	probeID_2;
    uint32_t	probeID_3;
    uint32_t 	uptime;
    uint16_t	PoE_voltage;
    uint32_t 	Reset_counter;
    uint32_t	MainLoop_speed;
    uint32_t	Accel_INT_cnt;
} __attribute__((packed)) Status_Packet;
#pragma pack(pop)


typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;
    uint8_t month;
    uint16_t year;
}FirmwareBuildDate_t;


#pragma pack(push,1)
typedef struct {
    uint16_t 	mark;
    uint8_t		type;
    uint16_t	data_len;
    uint8_t		head_crc;
} __attribute__((packed)) Packet_Header;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct {
    Packet_Header 	header;
    uint8_t 		crc;
    uint8_t			*data;
} __attribute__((packed)) Data_Packet;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct {
    Packet_Header header;
    uint8_t crc;
    uint16_t index;
    uint16_t block_len;
    char *data;
}__attribute__((packed))firmwarePacket_t;
#pragma pack(pop)


extern FirmwareBuildDate_t g_index;
extern Status_Packet g_status;

extern uint8_t calc_crc8(uint8_t	* data, uint16_t len);
extern void parse_packet();

extern void parse_status();
extern void get_firmware_index();
extern void parse_firmware_index();

extern void send_simple_packet(uint8_t packet_type);
extern void send_firmware_header(firmwareArgs_t *firmware);
extern void send_firmware_data(firmwareArgs_t *firmware);

#endif //UDP_FIRMWARE_UPLOADER_PACKET_H
