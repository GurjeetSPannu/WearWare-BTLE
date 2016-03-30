/**
 ****************************************************************************************
 *
 * @file pedometer_lib.h
 *
 * @brief Product related design header file.
 *
 * Copyright (C) WearWare 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef PEDOMETER_LIB_H_
#define PEDOMETER_LIB_H_

uint32_t getSteps(void);
uint32_t getDistance(void);
void basic_pedometer(int8_t xdat);
void better_pedometer(int8_t xdat);
void setup_fancy_pedometer(void);
void fancy_pedometer(int8_t xdat, int8_t ydat); 
void get_sample(unsigned int *xdat, unsigned int *ydat, unsigned int *zdat); 
char IsStep(float avg, float oldavg); 
//void display_prep(void); 
//long int bin_to_bcd(long int bin_no); 
//extern void display_data(void); // found in File display.c 
//extern void display_data_clear(void); // found in File display.c 

#endif
