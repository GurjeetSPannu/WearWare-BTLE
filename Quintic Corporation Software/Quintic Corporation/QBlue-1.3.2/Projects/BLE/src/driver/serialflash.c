/**
 ****************************************************************************************
 *
 * @file serialflash.c
 *
 * @brief Serial Flash driver for QN9020 (MX25L512_U2, W25X40_U4, S25FL008A_U3).
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
****************************************************************************************
* @addtogroup  SERIAL_FLASH
* @{
****************************************************************************************
*/

/*
* INCLUDE FILES
****************************************************************************************
*/
#include "serialflash.h"
#if CONFIG_ENABLE_DRIVER_SERIAL_FLASH==TRUE && CONFIG_ENABLE_ROM_DRIVER_SERIAL_FLASH==FALSE

/// Serial flash command list
uint8_t g_flash_cmd[MAX_FLASH_CMD_NUM]=
{
    0x05,       /*!< Read Status Register */
    0x06,       /*!< Write Enable */
    0x20,       /*!< Sector Erase */
    0x52,       /*!< Block Erase */
    0x60,       /*!< Chip Erase */
    0xB9,       /*!< Deep Power Down */
    0xAB,       /*!< Release form Deep Power Down */
    0x01,       /*!< reserved, the value is not 0x00 and 0xFF */
};

/// Serial flash exist flag
static bool s_have_flash_flag = true;

/*
* EXPORTED FUNCTION DEFINITIONS
****************************************************************************************
*/

/**
****************************************************************************************
* @brief Set Serial Flash controller clock
* @param[in]  clock_div        FLASH_CLK_DIV(clock), clock units is Hz
* @description
*  This functin is used to set serial flash controller work clock.
*****************************************************************************************
*/
void set_flash_clock(uint32_t clock_div)
{
    sf_ctrl_SetCRWithMask(QN_SF_CTRL, SF_CTRL_MASK_CLK_DIV, clock_div);
}

/**
****************************************************************************************
* @brief  Check flash is busy or not
* @return flash write operateion status
* @description
* Check flash status register's WIP before program, erase or write status register
*****************************************************************************************
*/
static bool is_flash_busy(void)
{
    uint32_t status;

    if (s_have_flash_flag)
    {
        // write flash cmd to flash control register
        sf_ctrl_SetCRWithMask(QN_SF_CTRL, SF_CTRL_MASK_RD_STAT_CMD, RD_FLASH_ST_CMD);

        // read the flash status via flash control register
        status = sf_ctrl_GetFlashSR(QN_SF_CTRL) & 0xFF;
        // WIP=1 is in write operation, WIP=0 is not in write operation.
        return (status & 0x01);
    }
    else
    {
        return false;
    }
}

/**
****************************************************************************************
* @brief  Enable Flash write operation
*****************************************************************************************
*/
static void flash_write_enable(void)
{
    while (is_flash_busy());  //wait flash free.
    sf_ctrl_SetCMD(QN_SF_CTRL, g_flash_cmd[WREN_CMD]);
}

/**
****************************************************************************************
* @brief  Read out flash ID
* @return Flash ID
* @description
*  This function is used to read serial flash ID, which consists of 3 or 4 bytes depending on difference vendor.
*****************************************************************************************
*/
uint32_t read_flash_id(void)
{
    uint32_t id;
    uint32_t active_id;

    id = sf_ctrl_GetFlashID(QN_SF_CTRL);
    active_id = id & 0xFFFFFF;
    if ((active_id > 0) && (active_id < 0xFFFFFF))
    {
        s_have_flash_flag = true;
    }
    else
    {
        s_have_flash_flag = false;
    }
    return id;
}

/**
****************************************************************************************
* @brief  Erase n sectors of flash
* @param[in]  addr        A23-A0 specified a valid 24bit address of a sector
* @param[in]  n           number of sector
* @description
*  This function is used to erase serial flash sector.
*****************************************************************************************
*/
void sector_erase_flash(uint32_t addr, uint32_t n)
{
    while (n--)
    {
        flash_write_enable(); //set the Write Enable Latch bit
        sf_ctrl_SetCmdAddr(QN_SF_CTRL, __REV(addr | (g_flash_cmd[SE_CMD]<<24)));
        addr += 4*1024;            //all flash sector erase command erasing size is 4K
    }
}

/**
****************************************************************************************
* @brief  Erase n blocks of flash
* @param[in]  addr        A23-A0 specified a valid 24bit address of a block.
* @param[in]  block_size  flash a block content size
* @param[in]  n           requirement erasing number blocks
* @description
*  This function is used to erase serial flash block.
*****************************************************************************************
*/
void block_erase_flash(uint32_t addr, uint32_t block_size, uint32_t n)
{
    while (n--)
    {
        flash_write_enable(); //set the Write Enable Latch bit
        sf_ctrl_SetCmdAddr(QN_SF_CTRL, __REV(addr | (g_flash_cmd[BE_CMD]<<24)));
        addr += block_size;
    }
}

/**
****************************************************************************************
* @brief  Erase whole flash with flash's Chip Erase command
* @description
*  This function is used to erase entire serial flash.
*****************************************************************************************
*/
void chip_erase_flash(void)
{
    flash_write_enable(); //set the Write Enable Latch bit
    sf_ctrl_SetCMD(QN_SF_CTRL, g_flash_cmd[CE_CMD]);
}

/**
****************************************************************************************
* @brief  Read data form flash
* @param[in]  addr        flash address(3 bytes)
* @param[in]  pBuf        pointer to read data buffer address
* @param[in]  nByte       read size, it must <= 256 and must be 4 integer times
* @description
*  This function is used to read data from serial flash.
*
* @note
*  1. The parameter "addr" note:
*       - When the address range is from 0x00 to 0x1000 (NVDS area), the address must be 4
*         integer times.
*       - When the address range is greater than or equal to 0x1000 (Code area), the
*         address must be 256 integer times. (Encryption request)
*  2. The parameter "nByte" note:
*       - When the address range is from 0x00 to 0x1000 (NVDS area), the size must be 4
*         integer times and less than or equal to 256.
*       - When the address range is greater than or equal to 0x1000 (Code area), the
*         size must be 256 bytes integer times. (Encryption request)
*****************************************************************************************
*/
void read_flash(uint32_t addr, uint32_t *pBuf, uint32_t nByte)
{
    addr += QN_FLASH_BASE;   //get the data register address of flash control register
    while(is_flash_busy());  //wait the flash is free.
    sf_ctrl_SetDataLen(QN_SF_CTRL, nByte);
    nByte >>= 2;               //nByte must is 4 integer times
    while (nByte--)
    {
        *pBuf++ = *(uint32_t *)addr;
        addr += 4;
    }
}

/**
****************************************************************************************
* @brief      Write data to flash
* @param[in]  addr        flash address(3 bytes)
* @param[in]  pBuf        pointer to write data address
* @param[in]  nByte       write size, it must <= 256 and must be 4 integer times
* @description
*  This function is used to write data to serial flash.
*
* @note
*  1. The parameter "addr" note:
*       - When the address range is from 0x00 to 0x1000 (NVDS area), the address must be 4
*         integer times.
*       - When the address range is greater than or equal to 0x1000 (Code area), the
*         address must be 256 integer times. (Encryption request)
*  2. The parameter "nByte" note:
*       - When the address range is from 0x00 to 0x1000 (NVDS area), the size must be 4
*         integer times and less than or equal to 256.
*       - When the address range is greater than or equal to 0x1000 (Code area), the
*         size must be 256 bytes. (Encryption request)
*****************************************************************************************
*/
void write_flash(uint32_t addr, const uint32_t *pBuf, uint32_t nByte)
{
    addr += QN_FLASH_BASE;     //get the data register address of flash control register
    sf_ctrl_SetDataLen(QN_SF_CTRL, nByte);
    flash_write_enable();      //set the Write Enable Latch bit
    nByte >>= 2;               //nByte must is 4 integer time.
    while (nByte--)
    {
        *(uint32_t *)addr = *pBuf++;
        addr += 4;
    }
}

#if 0
/**
****************************************************************************************
* @brief Power on/off flash
* @param[in]  on_or_off     FLASH_POWER_DOWN,FLASH_POWER_ON
* @description
*  This function is used to power on/off serial flash.
*****************************************************************************************
*/
void power_on_or_off_flash(enum POWER_TYPE on_or_off)
{
    uint32_t cmd;

    if (on_or_off == FLASH_POWER_ON)
    {
        cmd = g_flash_cmd[RDPD_CMD];
    }
    else
    {
         while (is_flash_busy());  //wait the flash is free when flash is entered power-down
         cmd = g_flash_cmd[DPD_CMD];
    }
    sf_ctrl_SetCMD(QN_SF_CTRL, cmd);
}
#endif

/**
****************************************************************************************
* @brief check whether flash is present
* @return ture or false
*****************************************************************************************
*/
bool is_flash_present(void)
{
    const uint32_t id = sf_ctrl_GetFlashID(QN_SF_CTRL) & 0xFFFFFF;

    s_have_flash_flag = (id > 0 && id < 0xFFFFFF) ? true : false;

    return s_have_flash_flag;
}

/**
****************************************************************************************
* @brief Power on serial flash.
*****************************************************************************************
*/
void power_on_flash(void)
{
    uint32_t time_out = 0;

    //Power on
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_FLASH_VCC_EN, MASK_ENABLE);

    // flash clock on
    flash_clock_on();

    //Wait flash is ready
    while(++time_out < 256 && !is_flash_present());
}

/**
****************************************************************************************
* @brief Power off serial flash.
*****************************************************************************************
*/
void power_off_flash(void)
{
    //wait the flash is free when flash is entered power-down
    while (is_flash_busy());

    //Power off
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_FLASH_VCC_EN, MASK_DISABLE);

    // flash clock off
    flash_clock_off();
}

#endif /* CONFIG_ENABLE_DRIVER_SERIAL_FLASH==TRUE */
/// @} SERIAL_FLASH
