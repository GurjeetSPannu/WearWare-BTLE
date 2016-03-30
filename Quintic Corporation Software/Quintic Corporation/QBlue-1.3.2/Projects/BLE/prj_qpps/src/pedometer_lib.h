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

int getSteps(void);
int getDistance(void);
void basic_pedometer(void);
void setup_fancy_pedometer(void);
void fancy_pedometer(void); 
void get_sample(unsigned int *xdat, unsigned int *ydat, unsigned int *zdat); 
char IsStep(float avg, float oldavg); 
//void display_prep(void); 
//long int bin_to_bcd(long int bin_no); 
//extern void display_data(void); // found in File display.c 
//extern void display_data_clear(void); // found in File display.c 

#endif
