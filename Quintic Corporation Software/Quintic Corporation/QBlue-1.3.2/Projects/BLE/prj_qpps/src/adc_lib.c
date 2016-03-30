/**
 ****************************************************************************************
 *
 * @file adc_lib.c
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
#include "system.h"
#include "app_env.h"
#include "i2c.h"
#include "adc_lib.h"
#include "analog.h"
#include "adc.h"
#include "sleep.h"
#include "lib.h"
#include "stdio.h"
#include "accel_lib.h"
#include "protocol_lib.h"





volatile uint32_t adc_done = 0;



/**
 ****************************************************************************************
 * @brief Measure Temperaturee with ADC
 ****************************************************************************************
 */
int8_t adc_get_temp(void)
{
	  int16_t tempv;
		
		// Read configuration
    adc_read_configuration read_cfg;
	
	
		temp_sensor_enable(MASK_ENABLE);
    //int16_t tempv;
    adc_init(ADC_DIFF_WITH_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
    adc_done = 0;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = BURST_MOD;
    read_cfg.start_ch = TEMP;
    read_cfg.end_ch = TEMP;
    adc_read(&read_cfg, &tempv, 1, adc_sample_complete_callback);
    while (adc_done == 0);
   // QPRINTF("temperature: %0.1f\r\n", (float)(TEMPERATURE_X10(tempv)/10.0));
	  int16_t tempp = (TEMPERATURE_X10(tempv)/10);
    QPRINTF("temperature: %d\r\n", tempp);

		return tempp;
}


/**
 ****************************************************************************************
 * @brief Measure Battery voltage with ADC
 ****************************************************************************************
 */
uint8_t adc_get_batt(void)
{
	  int16_t battv;
		uint8_t battp;
		
    adc_init(ADC_SINGLE_WITH_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
		// Read configuration
    adc_read_configuration read_cfg;
	
		battery_monitor_enable(MASK_ENABLE);
		adc_done = 0;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = BURST_MOD;
    read_cfg.start_ch = BATT;
    read_cfg.end_ch = BATT;
    adc_read(&read_cfg, &battv, 1, adc_sample_complete_callback);
    while (adc_done == 0);
		battv = 4*ADC_RESULT_mV(battv);
    //printf("battery voltage: %d\r\n", battv);
	
		if(battv <= BATT_FULLY_DISCHARGED_VOLTAGE)
    {
        battp = 0;
    }
    else if(battv >= BATT_FULLY_CHARGED_VOLTAGE)
    {
        battp = 100;
    }
    else
    {
        battp = (uint8_t)((battv - BATT_FULLY_DISCHARGED_VOLTAGE) * 100 /
                         (BATT_FULLY_CHARGED_VOLTAGE - BATT_FULLY_DISCHARGED_VOLTAGE));
    }
		QPRINTF("battery precentage: %d\r\n", battp);
	
		return battp;
}

/**
 ****************************************************************************************
 * @brief callback of adc sample complete.
 ****************************************************************************************
 */
void adc_sample_complete_callback(void)
{
    adc_done = 1;
}
