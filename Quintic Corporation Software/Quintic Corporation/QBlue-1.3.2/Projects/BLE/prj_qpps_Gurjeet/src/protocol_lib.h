/**
 ****************************************************************************************
 *
 * @file protocol_lib.h
 *
 * @brief Product related design header file.
 *
 * Copyright (C) WearWare 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef PROTOCOL_LIB_H_
#define PROTOCOL_LIB_H_
//Data IDs:
#define PROTOCOL_HW_VER						0x01
#define PROTOCOL_SW_VER						0x02
#define PROTOCOL_BATT							0x03
#define PROTOCOL_TEMP							0x04
#define PROTOCOL_ACCEL_VEC_XYZ		0x05
#define PROTOCOL_PED							0x06
#define PROTOCOL_DISTANCE					0x0C
#define PROTOCOL_ADC_SAMPLE				0x0D
#define PROTOCOL_ACCEL_VEC_X			0x0F

//Header IDs
#define PROTOCOL_COMMAND_ENABLE		0x07
#define PROTOCOL_COMMAND_DISABLE	0x08
#define PROTOCOL_COMMAND_RATE			0x09
#define PROTOCOL_COMMAND_REQUEST	0x0A // 
#define PROTOCOL_COMMAND_OTA			0x0B // not suppoted


//Tx Charactersitics: (only use TX2)
#define PROTOCOL_MODE_TX1					0x00
#define PROTOCOL_MODE_TX2					0x01

#define PROTOCOL_VERSION					0x01
#define PROTOCOL_VAL_HW_VER				0x01
#define PROTOCOL_VAL_SW_VER				0x01


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void send_packet(uint8_t mode);
void receive_packet(int len, char *packet);
uint8_t package_notification(uint8_t mode, char *packet);
uint8_t get_header_by_id(uint8_t id, char* data, uint8_t start);
uint8_t get_data_by_id(uint8_t id, char* data, uint8_t start);
int protocol_execute_command(char *data, int pos);

void restart_protocol_timer(void);
void sleep_timer_set(uint32_t time);

uint8_t create_list(uint8_t val);
uint8_t add_to_list(uint8_t val);
uint8_t search_in_list(uint8_t val);
uint8_t delete_from_list(uint8_t val);
void print_list(void);


#endif
