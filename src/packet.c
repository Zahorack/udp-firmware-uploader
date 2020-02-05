//
// Created by oholly on 5. 2. 2020.
//

#include "../include/packet.h"
#include "../include/socket.h"


#define		PACKET_MARK					0x4B6B
#define		PACKET_MAX_BLOCK_SIZE		1000


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


#pragma pack(push,1)
typedef struct {
    uint16_t 	mark;
    uint8_t		type;
    uint16_t	data_len;
    uint8_t		head_crc;
} __attribute__((packed)) Packet_Header;
#pragma pack(pop)
Packet_Header	* p_header_rx;

#pragma pack(push,1)
typedef struct {
    Packet_Header 	header;
    uint8_t 		crc;
    uint8_t			*data;
} __attribute__((packed)) Data_Packet;
#pragma pack(pop)

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



uint8_t calc_crc8(uint8_t	* data, uint16_t len){
    uint8_t 	crc = 0;
    uint16_t 	n;

    for(n = 0; n< len; n++){
        crc = crc8_Table[ crc ^ data[n] ] ;
    }

    return crc;
}


void send_simple_packet(uint8_t packet_type){

    uint16_t		packet_size = sizeof(Packet_Header);			// - pointer na data
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

        //udp_send_data((char *)p, packet_size);
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
                        sleep(1);
                        send_simple_packet(packet_type_header_ack);
                        break;
                }

            }
        }
    }
}