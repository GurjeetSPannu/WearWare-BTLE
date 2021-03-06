/**
 ****************************************************************************************
 *
 * @file wdt.c
 *
 * @brief watchdog timer driver for QN9020.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  WDT
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "wdt.h"
#if CONFIG_ENABLE_DRIVER_WDT==TRUE && CONFIG_ENABLE_ROM_DRIVER_WDT==FALSE
#ifdef BLE_PRJ
#include "usr_design.h"
#endif
volatile int reset_test = 0;  /*!< Set to 1 during watchdog timer reset test so that WDT_IRQHandler will not clear the watchdog */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Unlock watchdog timer access
 *
 ****************************************************************************************
 */
void wdt_unlock(void)
{
    QN_WDT->LOCK = 0x1ACCE551;
}

/**
 ****************************************************************************************
 * @brief Lock watchdog timer access
 *
 ****************************************************************************************
 */
void wdt_lock(void)
{
    QN_WDT->LOCK = 0;
}

/**
 ****************************************************************************************
 * @brief Clear watchdog timer interrupt request
 *
 ****************************************************************************************
 */
void wdt_irq_clear(void)
{
    wdt_unlock();
    QN_WDT->INTCLR = WDT_MASK_INTCLR;
    wdt_lock();
}


/**
 ****************************************************************************************
 * @brief Watchdog timer interrupt handler
 ****************************************************************************************
 */
#if CONFIG_WDT_DEFAULT_IRQHANDLER==TRUE
void WDT_IRQHandler(void)
{
    if (reset_test == 1){  /* When testing watchdog reset, need to stay  in WDT_IRQHandler until the watchdog overflows again */
        while (1) {
        // wait for reset...
        }
    }
#ifdef BLE_PRJ
#if (defined(QN_ADV_WDT))
    usr_env.adv_wdt_to();
#endif
#endif
    wdt_irq_clear(); /* Deassert Watchdog interrupt */

}
#endif /* CONFIG_WDT_DEFAULT_IRQHANDLER==TRUE */


/**
 ****************************************************************************************
 * @brief Watchdog timer initialization
 * @param[in]    cycle      time-out interval
 * @param[in]    mode       wrok mode: WDT_NO_ACTION_MOD/WDT_INT_MOD/WDT_RESET_MOD
 * @description
 *  This function is used to set WDT work mode and WDT time-out interval.
 *
 ****************************************************************************************
 */
void wdt_init(unsigned int cycle, enum WDT_MODE mode)
{
    wdt_clock_on();

    wdt_unlock();

    QN_WDT->LOAD = cycle;

    if (mode == WDT_NO_ACTION_MOD) {  /* Set watchdog to no action */
        QN_WDT->CTRL = 0;
    }
    else if (mode ==  WDT_INT_MOD) { /* Generate irq */
        QN_WDT->CTRL = WDT_MASK_CTRL_INTEN;
    }
    else if (mode ==  WDT_RESET_MOD) { /* Generate Reset */
        QN_WDT->CTRL = WDT_MASK_CTRL_RESEN | WDT_MASK_CTRL_INTEN;
        reset_test = 1;
    }

#if CONFIG_WDT_ENABLE_INTERRUPT==TRUE
    /* Enable the Watchdog TIMER Interrupt */
    NVIC_EnableIRQ(WDT_IRQn);
#endif

    wdt_lock();
}

/**
 ****************************************************************************************
 * @brief Update watchdog timer counter
 * @param[in]    cycle      time-out interval
 * @description
 *  This function is used to set WDT time-out interval.
 ****************************************************************************************
 */
void wdt_set(unsigned int cycle)
{
    wdt_unlock();
    QN_WDT->LOAD = cycle;
    wdt_lock();
}

#endif /* CONFIG_ENABLE_DRIVER_WDT==TRUE */
/// @} WDT
