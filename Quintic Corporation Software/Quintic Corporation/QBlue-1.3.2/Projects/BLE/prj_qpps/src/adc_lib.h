/**
 ****************************************************************************************
 *
 * @file battery_lib.h
 *
 * @brief Product related design header file.
 *
 * Copyright (C) WearWare 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef ADC_LIB_H_
#define ADC_LIB_H_

// The fully charged voltage and fully discharged voltage value, unit: mv
#define BATT_FULLY_CHARGED_VOLTAGE        3000
#define BATT_FULLY_DISCHARGED_VOLTAGE     2300


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern uint8_t adc_get_batt(void);
extern int8_t adc_get_temp(void);
extern void adc_sample_complete_callback(void);

#endif
