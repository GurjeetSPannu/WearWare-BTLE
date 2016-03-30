/**
 ****************************************************************************************
 *
 * @file accel_lib.c
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
#include "protocol_lib.h"
#include "pedometer_lib.h"
#include "accel_lib.h"
#include "sleep.h"
#include "lib.h"
 
/*uint8_t send_packet_bool = 0;

void send_packet_ready(void)
{
		send_packet_bool = 1;
}*/
 
 //get accelerometer samples, mask can choose which axis are requested (8b000_0zyx)
void accel_get_sample(int8_t *xdat, int8_t *ydat, int8_t *zdat, uint8_t mask)
{
	uint8_t i2cbuffer[3];

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 3);
	//for(int i = 0; i< 10000; i++){}
	if((mask & 0x01) > 0) //send x axis
		*xdat = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_X_H))));
	if((mask & 0x02) > 0) //send y axis
		*ydat = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Y_H))));
	if((mask & 0x04) > 0) //send z axis
		*zdat = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Z_H))));
}

void accel_get_X_samples(int8_t *xdat, uint8_t samps)
{
	uint8_t i2cbuffer[3];
	int i;

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 3);
	for(i = 0; i< 10000; i++);//wait for i2c clock to lock
	
	for(i = 0; i< samps; i++)
	{
		xdat[i] = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_X_H))));
	}
}

void accel_get_XY_samples(int8_t *xdat, int8_t *ydat, uint8_t samps)
{
	uint8_t i2cbuffer[3];
	int i;

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 3);
	for(i = 0; i< 10000; i++);//wait for i2c clock to lock
	
	for(i = 0; i< samps; i++)
	{
		xdat[i] = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_X_H))));
		ydat[i] = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Y_H))));
	}
}

void accel_gpio_init(void)
{
		//gpio_pull_set(ACCEL_INT_PIN, GPIO_PULL_DOWN);
		gpio_set_direction_field(ACCEL_INT_PIN, GPIO_INPUT);
	
		gpio_set_interrupt(ACCEL_INT_PIN, GPIO_INT_RISING_EDGE);
		gpio_enable_interrupt(ACCEL_INT_PIN);
	
		//Accelerometer interrupt
		//wakeup_by_gpio(ACCEL_INT_PIN, GPIO_WKUP_BY_HIGH); // does gpio_wakeup_config automatically
		gpio_wakeup_config(ACCEL_INT_PIN, GPIO_WKUP_BY_HIGH); //sets direction to input automatically

}

void accel_shutdown(void)
{
	uint8_t i2cbuffer[4];
	//Initialize I2C master
  i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, LIS3DH_POWER_DOWN);
	
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG5, LIS3DH_BOOT); //reboot regs
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG6, LIS3DH_BOOT_I1);
	
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, 0x00); //power down
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG2, 0x00);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG3, 0x00);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG4, 0x00);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG5, 0x00);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG6, 0x00);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_FIFO_CTRL_REG, 0x00);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_INT1_CFG, 0x00);
	
}

void accel_disable_int(void)
{
	uint8_t i2cbuffer[4];

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG3, 0x00); //turn off FIFO interrupt
}

//enable accelerometer for single sample, no interrupt
void accel_enable_sample(void)
{
	uint8_t i2cbuffer[4];
	
	accel_shutdown(); //reset accel

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, LIS3DH_50HZ | LIS3DH_LP | LIS3DH_ENX | LIS3DH_ENY | LIS3DH_ENZ);
	
	//original stuff for init below:
	//I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, LIS3DH_50HZ | LIS3DH_LP | LIS3DH_ENX | LIS3DH_ENY | LIS3DH_ENZ);
}

//enable accelerometer for pedometer
void accel_enable_fifo(void)
{
	uint8_t i2cbuffer[4];

	//QPRINTF("Configuring FIFO...");
	
	accel_shutdown(); //reset accel

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);

	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, LIS3DH_50HZ | LIS3DH_LP | LIS3DH_ENX | LIS3DH_ENY | LIS3DH_ENZ); //turn on XYZ, low power, 50Hz
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_MODE | ACCEL_FIFO_DEPTH); //fifo mode, watermark length 10, Send to inerrupt to INT1
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG5, LIS3DH_FIFO_EN | LIS3DH_LIR_INT1); //Enable FIFO
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG3, LIS3DH_FIFO_WM | LIS3DH_INT1_EN | LIS3DH_FIFO_OVERRUN); //Enable FIFO watermark interrupt 1
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG2, LIS3DH_HPF_EN); //enalbe high pass filter for pedometer, disable for better value range on all XYZ axis
	//I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_HP_FILTER_RESET); //reset high pass filter zero level to current
	//QPRINTF("Done\r\n");
}

void accel_enable_temp(void)
{
		uint8_t i2cbuffer[4];
	
		//QPRINTF("Configuring Temp Sensor...");
	
		//Initialize I2C master
    i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);
	
		I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_TEMP_CFG_REG, LIS3DH_ADC_EN | LIS3DH_TEMP_EN); //Enable ADC and Temp sensor
		//I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG4, LIS3DH_BDU); //Enable Block Data Update

		//QPRINTF("Done\r\n");
}

void accel_disable_temp(void)
{
		uint8_t i2cbuffer[4];
	
		//QPRINTF("Disabling Temp Sensor...");
	
		//Initialize I2C master
    i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);
	
		I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_TEMP_CFG_REG, 0x00); //disable ADC and Temp sensor

	//	QPRINTF("Done\r\n");
}

void accel_int_handler(void)
{
		// If BLE is in the sleep mode, wakeup it.
		if(ble_ext_wakeup_allow())
    {
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
        if (sleep_env.deep_sleep)
        {
            wakeup_32k_xtal_switch_clk();
        }
#endif

        sw_wakeup_ble_hw();
				
				//usr_sleep_restore();

    }
	
	ke_evt_set(1UL << EVENT_ACCEL_INT_ID);
}

void app_event_accel_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep)
    {
        sleep_env.deep_sleep = false;
        // start 32k xtal wakeup timer
        wakeup_32k_xtal_start_timer();
    }
#endif
		
		//QPRINTF(".");
		int8_t xdat[ACCEL_FIFO_DEPTH];
		//int8_t ydat[ACCEL_FIFO_DEPTH];
		accel_get_X_samples(xdat, ACCEL_FIFO_DEPTH);

		for(uint8_t i=0;i<ACCEL_FIFO_DEPTH;i++)
		{
			//fancy_pedometer(xdat[i], ydat[i]);
			//basic_pedometer();
			better_pedometer(xdat[i]);
		}
		
		/*if(send_packet_bool)
		{
			send_packet(PROTOCOL_MODE_TX2);
			send_packet_bool = 0;
		}*/
		
		ke_evt_clear(1UL << EVENT_ACCEL_INT_ID);
		
		sleep_set_pm(PM_SLEEP);

}
