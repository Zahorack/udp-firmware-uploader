//
// Created by oholly on 5. 2. 2020.
//

#include "../include/packet.h"
#include "../include/socket.h"
#include "../include/main.h"


#define		PACKET_MARK					0x4B6B
#define		PACKET_MAX_BLOCK_SIZE		1000

static uint16_t data_sent = 0;

Status_Packet g_status;
FirmwareBuildDate_t g_index;

const uint8_t crc8_Table[] =
{
        0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
        157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
        35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
        190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
        70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
        219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
        101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
        248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
        140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
        17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
        175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
        50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
        202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
        87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
        233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
        116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};




Packet_Header	* p_header_rx;


uint8_t calc_crc8(uint8_t	* data, uint16_t len){
    uint8_t 	crc = 0;
    uint16_t 	n;

    for(n = 0; n< len; n++){
        crc = crc8_Table[ crc ^ data[n] ] ;
    }
    return crc;
}


void send_simple_packet(uint8_t packet_type){

    uint16_t		packet_size = sizeof(Packet_Header);
    Data_Packet 	*p = malloc(packet_size);

    if(p != NULL){

        p->header.mark 		= PACKET_MARK;
        p->header.data_len 	= 1;
        p->header.type 		= packet_type;
        p->header.head_crc	= calc_crc8((uint8_t *)&p->header, sizeof(Packet_Header) - 1);
        p->crc				= 0;

        printf("-> send_simple_packet - p_size: %d, header: %d, %d, %d, crc: 0x%X\r\n",packet_size,  p->header.mark, p->header.data_len, p->header.type, p->header.head_crc);
//        printf("size of data packet: %d\n\r", sizeof(Data_Packet));
//        printf("size of header packet: %d\n\r", sizeof(Packet_Header));
//        printf("size of uint8: %d\n\r", sizeof(uint8_t));
//        printf("size of uint16: %d\n\r", sizeof(uint16_t));
        send_socket(&g_socket, (char*)p, packet_size);
    }
    else{
        printf("-> send_simple_packet - MEMORY PROBLEM !");
    }
    free(p);
}

void parse_packet()
{
    int ret = 0;
    char data[512];
    uint8_t		rx_head_crc = 0;
    uint8_t		rx_data_crc = 0;

    ret = receive_socket(&g_socket, data, 512);

    p_header_rx = (void*)data;

    if(ret) {
        rx_head_crc = calc_crc8((uint8_t *)p_header_rx, sizeof(Packet_Header) - 1);
        if(rx_head_crc == p_header_rx->head_crc) {
            if (p_header_rx->mark == PACKET_MARK) {
               // printf("packet type: 0x%X\r\n", p_header_rx->type);

                switch(p_header_rx->type) {

                    case packet_type_ack:
                        send_simple_packet(packet_type_header_ack);
                        break;
                    case packet_type_status:
                        send_simple_packet(packet_type_ack);
                        parse_status();

                        //testing now
                        get_firmware_index();
                        //if(je vyrmware stary, tak posli novy)
                        send_firmware_header(&g_firmware);
                        send_firmware_data(&g_firmware);
                        break;

                    case packet_type_firmware_index:
                        parse_firmware_index();
                        break;

                    default:
                        //printf("Unmknown packet type\n");
                        break;
                } //switch

            }
            else printf("Cannot catch packet mark!\n");
        }
        else printf("ERROR header CRC\n");
    }
}

void send_firmware_header(firmwareArgs_t *firmware)
{
    Data_Packet 	*p = malloc(sizeof(Data_Packet));

    if(p == NULL) {
        printf("Error! memory not allocated.\n\r");
    }

    p->data				= (uint8_t *)firmware->bin;
    p->crc 				= calc_crc8((uint8_t *)firmware->bin, firmware->size);

    p->header.mark 		= PACKET_MARK;
    p->header.data_len 	= firmware->size;
    p->header.type 		= packet_type_firmware_header;
    p->header.head_crc	= calc_crc8((uint8_t *)&p->header, sizeof(Packet_Header) - 1);

	//printf("\r\nheader: %d, %d, %d, crc: 0x%X\r\n", p->header.mark, p->header.data_len, p->header.type, p->header.head_crc);
    printf("-> send firmware header - data crc: 0x%X\r\n", p->crc);
    send_socket(&g_socket, (char*)p, sizeof(Packet_Header) +1); //+1 je este crc celeho balikui

    free(p);
    data_sent = 0;
    firmware->state = program_is_sending;

    //sent_time = HAL_GetTick();
}

void send_firmware_data(firmwareArgs_t *firmware)
{
    uint16_t data_size 	= firmware->size;
    uint16_t max_block_size = 900;
    uint16_t block_size;

    if((data_size - data_sent) > max_block_size){
        block_size =  max_block_size;
    }
    else{
        block_size = data_size - data_sent;
    }

    firmwarePacket_t 	*p = malloc(sizeof(firmwarePacket_t));

    if(p == NULL) {
        printf("Error! memory not allocated.\n\r");
    }

    p->header.mark 		= PACKET_MARK;
    p->header.data_len 	= firmware->size + 1;
    p->header.type 		= packet_type_firmware_data;
    p->header.head_crc	= calc_crc8((uint8_t *)&p->header, sizeof(Packet_Header) - 1);

    p->data				= (uint8_t *)firmware->bin + data_sent;
    p->crc 				= calc_crc8((uint8_t *)firmware->bin, firmware->size);
    p->block_len        = block_size;
    p->index            = firmware->block_index++;


//	DTRACE("-> 2 block_size: %d, data_sent: %d, data_size %d - %d\r\n", block_size, data_sent,data_size, (data_sent < data_size));

    if(data_sent < data_size) {
        //udp_send_data((char *)&g_ADCBuffer + data_sent, block_size) == SUCCESS);
        send_socket(&g_socket, (char*)p, sizeof(firmwarePacket_t) + block_size - 1);
        printf("-> send firmware data - data crc: 0x%X   block size: %d\r\n", p->crc, p->block_len);
    }
    else {
        firmware->state = program_was_send;
    }
}

uint32_t Reverse32(uint32_t value)
{
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) <<  8) |
            ((value & 0x00FF0000) >>  8) |
            ((value & 0xFF000000) >> 24));
}

void parse_status()
{
    uint16_t size = sizeof(Status_Packet);

    char data[256];
    int ret = receive_socket(&g_socket, data, 256);

    memcpy(&g_status, data, size);
    uint32_t real_ID = (uint32_t)Reverse32((g_status.probeID_1) ^ (g_status.probeID_2) ^ (g_status.probeID_3));

    printf("Probe status |ID : %d|   ", real_ID);
    printf("|Uptime: %d|      |Reset counter: %d| \n", g_status.uptime, g_status.Reset_counter);

}

void get_firmware_index()
{
    send_simple_packet(packet_type_firmware_index);
}

void parse_firmware_index()
{
    uint16_t size = sizeof(FirmwareBuildDate_t);

    char data[256];
    int ret = receive_socket(&g_socket, data, 256);

    memcpy(&g_index, data, size);

    printf("Firmware index ");
    printf("|DATE : %d.%d.%d|     ", g_index.day, g_index.month, g_index.year);
    printf("|TIME : %d:%d:%d|\n\r", g_index.hour, g_index.minute, g_index.second);

}