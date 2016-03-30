/**
 ****************************************************************************************
 *
 * @file protocol_lib.c
 *
 * @brief Product related design.
 *
 * Copyright (C) WearWare 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
 
 /*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include <stdint.h>
#include "app_env.h"
#include "i2c.h"
#include "sleep.h"
#include "lib.h"
#include "accel_lib.h"
#include "protocol_lib.h"
#include "pedometer_lib.h"
#include "adc_lib.h"
 
char list[20];

//sleep timer stuff
uint32_t protocol_UDR = 3000;
//static int wakeup_form_sleeptimer = 1; //set from interrupt routien
 
char packet[10] = {0,0,0,0,0,0,0,0,0,0};
 
 
 /**
 ****************************************************************************************
 * @brief   Protocol Functions
 ****************************************************************************************
 */
 
void send_packet(uint8_t mode)
{
	
	packet[0] = '\0'; //clear packet contents
	uint8_t len = package_notification(mode, packet);

	app_qpps_env->char_status &= ~(QPPS_VALUE_NTF_CFG << mode);
	app_qpps_data_send(app_qpps_env->conhdl, mode, len, (uint8_t *)packet);
	QPRINTF("Packet Sent: ");
	for(int i= 0; i<len; i++)
	{
		QPRINTF("%x:", packet[i]);
	}
	QPRINTF(" Len=%d\r\n", len);

	restart_protocol_timer();
}

void receive_packet(int len, char *packet)
{
	int pos = 0;
	QPRINTF("TEST: len=%d, I%02X\r\n", len, packet[0]);
	while(pos < len)
	{
		QPRINTF("command = %x\r\n", packet[pos]);
		int newlen = protocol_execute_command(packet, pos);
		if(newlen == -1)
		{
			//handle error!!! BARF
			ASSERT_ERR(0);
			pos = len;
		} else {
			pos += newlen;
		}
	}
}
	
uint8_t package_notification(uint8_t mode, char *packet)
{
	uint8_t ptr = 0;
	uint8_t len = 0;
	
	switch ( mode ) {
		case PROTOCOL_MODE_TX1:
			//version (hard coded beginig of packet)
			len += get_header_by_id(PROTOCOL_VERSION, packet, len);
		
		break;
		case PROTOCOL_MODE_TX2:
						
			while(list[ptr] != 0)
			{       
								
				//header
				len += get_header_by_id(list[ptr], packet, len);
				
				//data
				len += get_data_by_id(list[ptr], packet, len);

				ptr++;
			}
			packet[len] = '\0';
			QPRINTF("Done\r\n");
			
		break;
		default:
		// Code
		break;
	}
	return len;
}

uint8_t get_header_by_id(uint8_t id, char* data, uint8_t start)
{
	data[start] = id;
	return 1;
}
uint8_t get_data_by_id(uint8_t id, char* data, uint8_t start)
{
	uint8_t tot = 0; //number of bytes added to packet
	int8_t x, y, z;
	int udr = 0;
	
	switch ( id ) {
		case PROTOCOL_HW_VER:
			data[start] = PROTOCOL_VAL_HW_VER;
			tot = 1;
		break;
		case PROTOCOL_SW_VER:	
			data[start] = PROTOCOL_VAL_SW_VER;
			tot = 1;			
		break;
		case PROTOCOL_BATT:	
			data[start] = adc_get_batt();
			tot = 1;
		break;
		case PROTOCOL_TEMP:	
			data[start] = adc_get_temp();
			tot = 1;	
		break;
		case PROTOCOL_ACCEL_VEC_XYZ:
			accel_get_sample(&x,&y,&z);			
			data[start] = x;
			data[start+1] = y;
			data[start+2] = z;
			tot = 3;
		break;
		case PROTOCOL_PED:
			data[start] = (uint8_t)getSteps();
			tot = 1;
		break;
		case PROTOCOL_COMMAND_RATE:		
			udr = protocol_UDR/100;
			data[start] = (udr >> 24) & 0xFF;
			data[start+1] = (udr >> 16) & 0xFF;
			data[start+2] = (udr >> 8) & 0xFF;
			data[start+3] = (udr >> 0) & 0xFF;
			tot = 4;		
			//TODO: fix this, should be 32 bit	
		break;
		default:
			QPRINTF("Unrecognized ID: %x\r\n", id);
		break;
	}
	return tot;
}

int protocol_execute_command(char *data, int pos)
{
	uint32_t r = 0;
	int ret = 0;
	
	switch ( data[0+pos] ) {
		case PROTOCOL_COMMAND_ENABLE:	
			if(add_to_list(data[1+pos]) == 0xFF)
			{
				ASSERT_ERR(0);
				ret = -1;
			}
			
			if(data[1+pos] == PROTOCOL_PED)
			{
				accel_configure_int();
				accel_enable_fifo();
				setup_fancy_pedometer();
			}
			
			ret = 2;
		break;
		case PROTOCOL_COMMAND_DISABLE:	
			if( delete_from_list(data[1+pos]) == NULL)
			{
				ASSERT_ERR(0);
				ret = -1;
			}
			if(data[1+pos] == PROTOCOL_PED)
			{
				accel_shutdown();
			}
			ret = 2;		
		break;
		case PROTOCOL_COMMAND_RATE:		
			r |= ( data[1+pos] << 24 );
			r |= ( data[2+pos] << 16 );
			r |= ( data[3+pos] <<  8 );
			r |= ( data[4+pos]       );	
			protocol_UDR = r*100; //10ms to s
			//sleep_timer_set(protocol_UDR);
//			wakeup_form_sleeptimer = 0;
			QPRINTF("Setting timer for %d seconds...", r);
			//ke_timer_set(APP_SYS_BUTTON_1_TIMER, TASK_APP, 2);
			//ke_timer_set(APP_SYS_SLEEP_TIMER, TASK_APP, protocol_UDR);
			restart_protocol_timer();
			QPRINTF("done\r\n");
		
			ret = 5;		
		break;
		case PROTOCOL_COMMAND_REQUEST:
			if(data[1+pos] == PROTOCOL_MODE_TX1)
			{
				send_packet(PROTOCOL_MODE_TX1);
			}
			else if(data[1+pos] == PROTOCOL_MODE_TX2)
			{
				send_packet(PROTOCOL_MODE_TX2);
			}
			else
			{
				QPRINTF("Protocol Char. Not Supported\r\n");
			}
			ret = 2;
		break;
		case PROTOCOL_COMMAND_OTA:
			QPRINTF("Command Not Supported\r\n");
			//ASSERT_ERR(0);
			ret = -1;
		break;
		default:
			QPRINTF("Command Not Supported\r\n");
			//ASSERT_ERR(0);
			ret= 1; //skip the sommand byte
		break;
	}
	return ret;
}

void restart_protocol_timer(void)
{
	ke_timer_set(APP_SYS_SLEEP_TIMER, TASK_APP, protocol_UDR);
}

/**
 ****************************************************************************************
 * @brief   Sleep Timer Helper functions
 ****************************************************************************************
 */

void sleep_timer_set(uint32_t time)
{
    // TBC
    
    // ble interrupt mask
    *(volatile uint32_t *)(0x2f00000c) |= 0x4;
    
    // correction sleep time
    *(volatile uint32_t *)(0x2f000030) = (*(volatile uint32_t *)(0x2f000030) & (~0x8)) | 0x8;

    // sleep time
    *(volatile uint32_t *)(0x2f000034) =  time;
    
    // enable sleep
    *(volatile uint32_t *)(0x2f000030) = (*(volatile uint32_t *)(0x2f000030) & (~0x4)) | 0x4;

    // OSC SLEEP EN
    *(volatile uint32_t *)(0x2f000030) = (*(volatile uint32_t *)(0x2f000030) & (~0x3)) | 0x1;

    //delay(5000);
    while (syscon_GetBLESR(QN_SYSCON) & SYSCON_MASK_OSC_EN);
}
 
 /**
 ****************************************************************************************
 * @brief   Linked List helpers
 ****************************************************************************************
 */
uint8_t create_list(uint8_t val)
{
    QPRINTF("creating list with headnode as [%d]\r\n",val);
    list[0] = val;
		list[1] = '\0';
	return 0;
}

//Add only unique items to list
uint8_t add_to_list(uint8_t val)
{			
	
    if(list[0] == 0)
    {
        return (create_list(val));
    }

    if(search_in_list(val) != 0xFF)
    {
			QPRINTF("Item already in list\r\n");
      return search_in_list(val);
    }

		list[strlen(list)] = val;
		list[strlen(list)+1] = 0;
    
    return 0;
}

uint8_t search_in_list(uint8_t val)
{
    bool found = false;
		uint8_t ptr = 0;

    //QPRINTF("Searching the list for value [%d] \r\n",val);

    while(list[ptr] != 0)
    {
        if(list[ptr] == val)
        {
            found = true;
            break;
        }
        else
        {
            ptr++;
        }
    }

    if(true == found)
    {
        return ptr;
    }
    else
    {
        return 0xFF;
    }
}

uint8_t delete_from_list(uint8_t val)
{

    QPRINTF("Deleting value [%d] from list\r\n",val);

    uint8_t del = search_in_list(val);
    if(del == 0xFF)
    {
        return 0xFF;
    }
    else
    {
				list[del] = 0;
        while(list[del+1] != 0)
				{
					list[del] = list[del+1];
					del++;
				}
    }
    return 0;
}

void print_list(void)
{
    uint8_t ptr = 0;

    QPRINTF("-------Printing list Start------- \r\n");
    while(list[ptr] != 0)
    {
        QPRINTF("\n [%d] \n",list[ptr]);
        ptr++;
    }
    QPRINTF("-------Printing list End------- \r\n");

    return;
}
