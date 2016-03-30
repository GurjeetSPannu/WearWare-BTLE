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
 

 
void accel_get_sample(int8_t *xdat, int8_t *ydat, int8_t *zdat)
{
	uint8_t i2cbuffer[3];

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 3);
	*xdat = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_X_H))));
	*ydat = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Y_H))));
	//*zdat = (int8_t)(((uint8_t)(I2C_BYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Z_H))));
	//I2C_nBYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_X_H, i2cbuffer, 2);
	//I2C_nBYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Y_H, i2cbuffer, 2);
	//I2C_nBYTE_READ(LIS3DH_ADDR, LIS3DH_OUT_Z_H, i2cbuffer, 2);
	//QPRINTF("%x, %x, %x\r\n", *xdat, *ydat, *zdat);

}

 /**
 ****************************************************************************************
 * @brief   Test i2c port with accel
 ****************************************************************************************
 */
void test_i2c(void)
{
		//accel_enable_fifo();
		//setup_fancy_pedometer();
	/*while (1) 
	{ 
		fancy_pedometer();
	}*/
		//basic_pedometer();
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
}

void accel_disable_int(void)
{
	uint8_t i2cbuffer[4];

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG3, 0x00); //turn off FIFO interrupt
}
void accel_configure_int(void)
{
	uint8_t i2cbuffer[4];

	QPRINTF("Configuring Interrupt...");

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);

	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, LIS3DH_POWER_DOWN);
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG5, LIS3DH_BOOT); //reboot regs
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG6, LIS3DH_BOOT_I1);
	
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG1, LIS3DH_50HZ | LIS3DH_LP | LIS3DH_ENX | LIS3DH_ENY);// | LIS3DH_ENZ); //turn on XYZ, low power, 50Hz: 01000111
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG3, LIS3DH_INT1_EN); //Send inerrupt to INT1
	//I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_INT1_CFG, 0x20); //configure inturrupt (high Z event): 00100000
	//I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_INT1_THS, 0xF0); //configure inturrupt threshold: 11110000

	QPRINTF("Done\r\n");
}

void accel_enable_fifo(void)
{
	uint8_t i2cbuffer[4];

	QPRINTF("Configuring FIFO...");

	//Initialize I2C master
	i2c_init(I2C_SCL_RATIO(1), i2cbuffer, 4);

	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_MODE | ACCEL_FIFO_DEPTH); //fifo mode, watermark length 10, Send to inerrupt to INT1
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG5, LIS3DH_FIFO_EN | LIS3DH_LIR_INT1); //Enable FIFO
	I2C_BYTE_WRITE(LIS3DH_ADDR, LIS3DH_CTRL_REG3, LIS3DH_FIFO_WM | LIS3DH_INT1_EN | LIS3DH_FIFO_OVERRUN); //Enable FIFO watermark interrupt 1
	QPRINTF("Done\r\n");
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
		
		QPRINTF(".");
		for(uint8_t i=0;i<ACCEL_FIFO_DEPTH;i++)
		{
			fancy_pedometer();
		}
		
		ke_evt_clear(1UL << EVENT_ACCEL_INT_ID);
		
		sleep_set_pm(PM_DEEP_SLEEP);

}
