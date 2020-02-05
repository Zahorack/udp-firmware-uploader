//
// Created by oholly on 5. 2. 2020.
//

#ifndef UDP_FIRMWARE_UPLOADER_PACKET_H
#define UDP_FIRMWARE_UPLOADER_PACKET_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
    packet_type_clear_davis_ID
};


extern uint8_t calc_crc8(uint8_t	* data, uint16_t len);
extern void parse_packet();

extern void send_simple_packet(uint8_t packet_type);

#endif //UDP_FIRMWARE_UPLOADER_PACKET_H
