/**
 ****************************************************************************************
 *
 * @file accel_lib.h
 *
 * @brief Product related design header file.
 *
 * Copyright (C) WearWare 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef ACCEL_LIB_H_
#define ACCEL_LIB_H_

#define ACCEL_INT_PIN 					GPIO_P04

#define EVENT_ACCEL_INT_ID 			12

#define ACCEL_FIFO_DEPTH 				20

#define LIS3DH_ADDR 						0x19

#define LIS3DH_BOOT							0x80
#define LIS3DH_BOOT_I1					0x10
#define LIS3DH_100HZ 						0x50
#define LIS3DH_50HZ 						0x40
#define LIS3DH_25HZ 						0x30
#define LIS3DH_10HZ 						0x20
#define LIS3DH_1HZ 							0x10
#define LIS3DH_LP 							0x08
#define LIS3DH_ENX 							0x01
#define LIS3DH_ENY 							0x02
#define LIS3DH_ENZ 							0x04
#define LIS3DH_POWER_DOWN 			0x00
#define LIS3DH_LIR_INT1					0x10

#define LIS3DH_FIFO_MODE 				0x40
#define LIS3DH_FIFO_EN 					0x40
#define LIS3DH_FIFO_WM					0x04
#define LIS3DH_INT1_EN					0x40
#define LIS3DH_FIFO_OVERRUN			0x02

#define LIS3DH_ADC_EN						0x80
#define LIS3DH_TEMP_EN					0x40
#define LIS3DH_BDU							0x40

#define LIS3DH_STATUS_REG_AUX 	0x07
#define LIS3DH_OUT_ADC1_L 			0x08
#define LIS3DH_OUT_ADC1_H 			0x09
#define LIS3DH_OUT_ADC2_L 			0x0A
#define LIS3DH_OUT_ADC2_H 			0x0B
#define LIS3DH_OUT_ADC3_L 			0x0C
#define LIS3DH_OUT_ADC3_H 			0x0D
#define LIS3DH_INT_COUNTER_REG 	0x0E
#define LIS3DH_WHO_AM_I 				0x0F
#define LIS3DH_TEMP_CFG_REG 		0x1F
#define LIS3DH_CTRL_REG1 				0x20
#define LIS3DH_CTRL_REG2 				0x21
#define LIS3DH_CTRL_REG3 				0x22
#define LIS3DH_CTRL_REG4 				0x23
#define LIS3DH_CTRL_REG5 				0x24
#define LIS3DH_CTRL_REG6 				0x25
#define LIS3DH_REFERENCE 				0x26
#define LIS3DH_STATUS_REG2 			0x27
#define LIS3DH_OUT_X_L 					0x28
#define LIS3DH_OUT_X_H 					0x29
#define LIS3DH_OUT_Y_L 					0x2A
#define LIS3DH_OUT_Y_H 					0x2B
#define LIS3DH_OUT_Z_L 					0x2C
#define LIS3DH_OUT_Z_H 					0x2D
#define LIS3DH_FIFO_CTRL_REG 		0x2E
#define LIS3DH_FIFO_SRC_REG 		0x2F
#define LIS3DH_INT1_CFG 				0x30
#define LIS3DH_INT1_SOURCE 			0x31
#define LIS3DH_INT1_THS 				0x32
#define LIS3DH_INT1_DURATION 		0x33
#define LIS3DH_CLICK_CFG 				0x38
#define LIS3DH_CLICK_SRC 				0x39
#define LIS3DH_CLICK_THS 				0x3A
#define LIS3DH_TIME_LIMIT 			0x3B
#define LIS3DH_TIME_LATENCY 		0x3C
#define LIS3DH_TIME_WINDOW 			0x3D


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void accel_get_sample(int8_t *xdat, int8_t *ydat, int8_t *zdat);
void test_i2c(void);
void accel_gpio_init(void);
void accel_shutdown(void);
void accel_disable_int(void);
void accel_configure_int(void);
void accel_enable_fifo(void);
void accel_enable_temp(void);
void accel_disable_temp(void);
void accel_int_handler(void);
void app_event_accel_handler(void);

#endif
