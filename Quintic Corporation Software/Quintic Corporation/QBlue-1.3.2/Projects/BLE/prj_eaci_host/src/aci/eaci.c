/**
 ****************************************************************************************
 *
 * @file eaci.c
 *
 * @brief Easy ACI interface module source file.
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// EACI environment context
struct eaci_env_tag eaci_env;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize EACI interface
 *
 ****************************************************************************************
 */
void app_eaci_init(void)
{
    LPC_PINCON->PINSEL3 &= ~(0x03 << 2);                                       /* ��P1.17��ʼ��ΪGPIO����      */
    LPC_GPIO1->FIODIR    |= (1 << 17);                                       /* ��P1.17��������Ϊ���        */
    LPC_GPIO1->FIOSET    |= (1 << 17);                                       /* ��P1.17��ʼ�������ƽ*/
    eaci_uart_init();
}

/**
 ****************************************************************************************
 * @brief EACI application message handler
 *
 ****************************************************************************************
 */
void app_eaci_msg_hdl(uint8_t msg_type, uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_type)
    {
    case EACI_MSG_TYPE_EVT:
        app_eaci_evt(msg_id, param_len, param);
        break;
    case EACI_MSG_TYPE_DATA_IND:
        app_eaci_data_ind(msg_id, param_len, param);
        break;
    case EACI_MSG_TYPE_DATA_ERROR:
        app_eaci_data_error_rsp(msg_id, param_len, param);
        break;
    case EACI_MSG_TYPE_CMD:
    case EACI_MSG_TYPE_DATA_REQ:
    default:
        break;
    }
}

/// @} EACI
