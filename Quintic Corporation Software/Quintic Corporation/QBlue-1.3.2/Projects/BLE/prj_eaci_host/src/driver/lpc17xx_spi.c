/**
 * @file	: lpc17xx_spi.c
 * @brief	: Contains all functions support for SPI firmware library on LPC17xx
 * @version	: 1.0
 * @date	: 3. April. 2009
 * @author	: HieuNguyen
 **************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup SPI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_spi.h"
#include "lpc17xx_clkpwr.h"
#include "string.h"
#include "lpc17xx_gpio.h"

extern void CS_Force(int32_t state);
extern SPI_DATA_SETUP_Type xferConfig;
volatile uint8_t rd_ongoing = 0;
volatile uint8_t wr_ongoing = 0;
uint8_t tx_buffer[80];

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */

#ifdef _SPI

/* Private Types -------------------------------------------------------------- */
/** @defgroup SPI_Private_Types
 * @{
 */

/** @brief SPI device configuration structure type */
typedef struct
{
	int32_t 	dataword;				/* Current data word: 0 - 8 bit; 1 - 16 bit */
	uint32_t    txrx_setup; 			/* Transmission setup */
	void		(*inthandler)(void);   	/* Transmission interrupt handler */
} SPI_CFG_T;

/**
 * @}
 */


/* Private Variables ---------------------------------------------------------- */
/* SPI configuration data */
static SPI_CFG_T spidat;


/* Private Functions ---------------------------------------------------------- */
/** @defgroup SPI_Private_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Standard Private SPI Interrupt handler
 * @param[in]	None
 * @return 		None
 ***********************************************************************/
void SPI_IntHandler(void)
{
	SPI_DATA_SETUP_Type *xf_setup;
    uint16_t tmp;
    xf_setup = (SPI_DATA_SETUP_Type *)spidat.txrx_setup;
    CS_Force(1);
    /* Dummy read to clear SPI interrupt flag */
    if (LPC_SPI->SPINT & SPI_SPINT_INTFLAG){
    	LPC_SPI->SPINT = SPI_SPINT_INTFLAG;
    }

    // save status
    tmp = LPC_SPI->SPSR;
    xf_setup->status = tmp;
    // Check for error
    if (tmp & (SPI_SPSR_ABRT | SPI_SPSR_MODF | SPI_SPSR_ROVR | SPI_SPSR_WCOL)){
    	xf_setup->status |= SPI_STAT_ERROR;
    	// Disable Interrupt and call call-back
    	SPI_IntCmd(LPC_SPI, DISABLE);
    	if (xf_setup->callback != NULL){
    		xf_setup->callback();
    	}
    	return;
    }

    /* Check SPI complete flag */
    if (tmp & SPI_SPSR_SPIF){
	   // Read data from SPI data
		tmp = SPI_ReceiveData(LPC_SPI);
		if (xf_setup->rx_data != NULL)
		{
//			if (spidat.dataword == 0){
//				*(uint8_t *)(xf_setup->rx_data + xf_setup->counter) = (uint8_t) tmp;
//			} else {
//				*(uint16_t *)(xf_setup->rx_data + xf_setup->counter) = (uint8_t) tmp;
//			}
			if (spidat.dataword == 0){
				*(uint8_t *)((uint8_t *)(xf_setup->rx_data) + xf_setup->counter) = (uint8_t) tmp;
			} else {
				*(uint16_t *)((uint8_t *)(xf_setup->rx_data) + xf_setup->counter) = (uint8_t) tmp;
			}
		}
        if (xf_setup->rd_length) {
            xf_setup->rd_length--;        
        }
        if(xf_setup->rd_length==0)
        {
            xf_setup->rx_data = NULL;
        }
        
        /*
        if (!(GPIO_ReadValue(0) & (1<<3))||(xf_setup->tx_data != NULL)) {
            if (spidat.dataword == 0){
                xf_setup->counter++;
            } else {
                xf_setup->counter += 2;
            }
        }*/
        
        if ((xf_setup->rd_length == 0) && (xf_setup->wr_length == 0)) {
            xf_setup->counter = xf_setup->length;
        }
        else {
            // Increase counter
            if (spidat.dataword == 0){
                xf_setup->counter++;
            } else {
                xf_setup->counter += 2;
            }
        }
    }

    if (xf_setup->counter < xf_setup->length) {
        
        if (xf_setup->wr_length) {
            xf_setup->wr_length--;
        }
        if(xf_setup->wr_length==0)
        {
            xf_setup->tx_data = NULL;
        }
		// Write data to buffer
		if(xf_setup->tx_data == NULL){
			if (spidat.dataword == 0){
				SPI_SendData(LPC_SPI, 0xFF);
			} else {
				SPI_SendData(LPC_SPI, 0xFFFF);
			}
		} else {
//			if (spidat.dataword == 0){
//				SPI_SendData(SPI, (*(uint8_t *)(xf_setup->tx_data + xf_setup->counter)));
//			} else {
//				SPI_SendData(SPI, (*(uint16_t *)(xf_setup->tx_data + xf_setup->counter)));
//			}
			if (spidat.dataword == 0){
				SPI_SendData(LPC_SPI, (*(uint8_t *)((uint8_t *)(xf_setup->tx_data) + xf_setup->counter)));
			} else {
				SPI_SendData(LPC_SPI, (*(uint16_t *)((uint8_t *)(xf_setup->tx_data) + xf_setup->counter)));
			}
		}
    }
    // No more data to send
	else {
    	xf_setup->status |= SPI_STAT_DONE;
    	// Disable Interrupt and call call-back
    	SPI_IntCmd(LPC_SPI, DISABLE);
    	if (xf_setup->callback != NULL){
    		xf_setup->callback();
    	}
	}
}


/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SPI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Setup clock rate for SPI device
 * @param[in] 	SPIx	SPI peripheral definition, should be SPI
 * @param[in]	target_clock : clock of SPI (Hz)
 * @return 		None
 ***********************************************************************/
void SPI_SetClock (LPC_SPI_TypeDef *SPIx, uint32_t target_clock)
{
	uint32_t spi_pclk;
	uint32_t prescale, temp;


	CHECK_PARAM(PARAM_SPIx(SPIx));

	if (SPIx == LPC_SPI){
		spi_pclk =  CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SPI);
	} else {
		return;
	}

	prescale = 8;
	// Find closest clock to target clock
	while (1){
		temp = target_clock * prescale;
		if (temp >= spi_pclk){
			break;
		}
		prescale += 2;
		if(prescale >= 254){
			break;
		}
	}
	// Write to register
	SPIx->SPCCR = SPI_SPCCR_COUNTER(prescale);
}


/*********************************************************************//**
 * @brief		De-initializes the SPIx peripheral registers to their
*                  default reset values.
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @return 		None
 **********************************************************************/
void SPI_DeInit(LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	if (SPIx == LPC_SPI){
		/* Set up clock and power for SPI module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, DISABLE);
	}
}



/********************************************************************//**
 * @brief		Initializes the SPIx peripheral according to the specified
*               parameters in the UART_ConfigStruct.
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @param[in]	SPI_ConfigStruct Pointer to a SPI_CFG_Type structure
*                    that contains the configuration information for the
*                    specified SPI peripheral.
 * @return 		None
 *********************************************************************/
void SPI_Init(LPC_SPI_TypeDef *SPIx, SPI_CFG_Type *SPI_ConfigStruct)
{
	uint32_t tmp;

	CHECK_PARAM(PARAM_SPIx(SPIx));

	if(SPIx == LPC_SPI){
		/* Set up clock and power for UART module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, ENABLE);
	} else {
		return;
	}

	// Configure SPI, interrupt is disable as default
	tmp = ((SPI_ConfigStruct->CPHA) | (SPI_ConfigStruct->CPOL) \
		| (SPI_ConfigStruct->DataOrder) | (SPI_ConfigStruct->Databit) \
		| (SPI_ConfigStruct->Mode) | SPI_SPCR_BIT_EN) & SPI_SPCR_BITMASK;
    tmp |= 0x800;
	// write back to SPI control register
	SPIx->SPCR = tmp;

	if (SPI_ConfigStruct->Databit > SPI_DATABIT_8){
		spidat.dataword = 1;
	} else {
		spidat.dataword = 0;
	}

	// Set clock rate for SPI peripheral
	SPI_SetClock(SPIx, SPI_ConfigStruct->ClockRate);

	// If interrupt flag is set, Write '1' to Clear interrupt flag
	if (SPIx->SPINT & SPI_SPINT_INTFLAG){
		SPIx->SPINT = SPI_SPINT_INTFLAG;
	}
}



/*****************************************************************************//**
* @brief		Fills each SPI_InitStruct member with its default value:
* 				- CPHA = SPI_CPHA_FIRST
* 				- CPOL = SPI_CPOL_HI
* 				- ClockRate = 1000000
* 				- DataOrder = SPI_DATA_MSB_FIRST
* 				- Databit = SPI_DATABIT_8
* 				- Mode = SPI_MASTER_MODE
* @param[in]	SPI_InitStruct Pointer to a SPI_CFG_Type structure
*                    which will be initialized.
* @return		None
*******************************************************************************/
void SPI_ConfigStructInit(SPI_CFG_Type *SPI_InitStruct)
{
	SPI_InitStruct->CPHA = 0;
	SPI_InitStruct->CPOL = 0;
	SPI_InitStruct->ClockRate = 200000;
	SPI_InitStruct->DataOrder = SPI_DATA_MSB_FIRST; 
	SPI_InitStruct->Databit = SPI_DATABIT_8;
	SPI_InitStruct->Mode = SPI_MASTER_MODE;
}

/*********************************************************************//**
 * @brief		Transmit a single data through SPIx peripheral
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @param[in]	Data	Data to transmit (must be 16 or 8-bit long,
 * 						this depend on SPI data bit number configured)
 * @return 		none
 **********************************************************************/
void SPI_SendData(LPC_SPI_TypeDef* SPIx, uint16_t Data)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));
    CS_Force(0);
    for(int i =0;i<1000;i++); //9600
    //for(int i =0;i<300;i++); //115200, controller need configure system clock to 16MHz
	SPIx->SPDR = Data & SPI_SPDR_BITMASK;
    
}



/*********************************************************************//**
 * @brief		Receive a single data from SPIx peripheral
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @return 		Data received (16-bit long)
 **********************************************************************/
uint16_t SPI_ReceiveData(LPC_SPI_TypeDef* SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return ((uint16_t) (SPIx->SPDR & SPI_SPDR_BITMASK));
}

/*********************************************************************//**
 * @brief 		SPI 	Read write data function
 * @param[in]	SPIx 	Pointer to SPI peripheral, should be SPI
 * @param[in]	dataCfg	Pointer to a SPI_DATA_SETUP_Type structure that
 * 						contains specified information about transmit
 * 						data configuration.
 * @param[in]	xfType	Transfer type, should be:
 * 						- SPI_TRANSFER_POLLING: Polling mode
 * 						- SPI_TRANSFER_INTERRUPT: Interrupt mode
 * @return 		Actual Data length has been transferred in polling mode.
 * 				In interrupt mode, always return (0)
 * 				Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SPI_ReadWrite (LPC_SPI_TypeDef *SPIx, SPI_DATA_SETUP_Type *dataCfg, \
						SPI_TRANSFER_Type xfType)
{
	uint8_t *rdata8;
    uint8_t *wdata8;
	uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t temp;

	//read for empty buffer
	temp = SPIx->SPDR;
	//dummy to clear status
	temp = SPIx->SPSR;
	dataCfg->counter = 0;
	dataCfg->status = 0;

	if (xfType == SPI_TRANSFER_POLLING){

		if (spidat.dataword == 0){
			rdata8 = (uint8_t *)dataCfg->rx_data;
			wdata8 = (uint8_t *)dataCfg->tx_data;
		} else {
			rdata16 = (uint16_t *)dataCfg->rx_data;
			wdata16 = (uint16_t *)dataCfg->tx_data;
		}

		while(dataCfg->counter < dataCfg->length)
		{
			// Write data to buffer
			if(dataCfg->tx_data == NULL){
				if (spidat.dataword == 0){
					SPI_SendData(SPIx, 0xFF);
				} else {
					SPI_SendData(SPIx, 0xFFFF);
				}
			} else {
				if (spidat.dataword == 0){
					SPI_SendData(SPIx, *wdata8);
					wdata8++;
				} else {
					SPI_SendData(SPIx, *wdata16);
					wdata16++;
				}
			}
			// Wait for transfer complete
			while (!((stat = SPIx->SPSR) & SPI_SPSR_SPIF));
			// Check for error
			if (stat & (SPI_SPSR_ABRT | SPI_SPSR_MODF | SPI_SPSR_ROVR | SPI_SPSR_WCOL)){
				// save status
				dataCfg->status = stat | SPI_STAT_ERROR;
				return (dataCfg->counter);
			}
			// Read data from SPI dat
			temp = (uint32_t) SPI_ReceiveData(SPIx);

			// Store data to destination
			if (dataCfg->rx_data != NULL)
			{
				if (spidat.dataword == 0){
					*(rdata8) = (uint8_t) temp;
					rdata8++;
				} else {
					*(rdata16) = (uint16_t) temp;
					rdata16++;
				}
			}
			// Increase counter
			if (spidat.dataword == 0){
				dataCfg->counter++;
			} else {
				dataCfg->counter += 2;
			}
		}

		// Return length of actual data transferred
		// save status
		dataCfg->status = stat | SPI_STAT_DONE;
		return (dataCfg->counter);
	}
	// Interrupt mode
	else {
		spidat.txrx_setup = (uint32_t)dataCfg;
		spidat.inthandler = SPI_IntHandler;

		// Check if interrupt flag is already set
		if(SPIx->SPINT & SPI_SPINT_INTFLAG){
			SPIx->SPINT = SPI_SPINT_INTFLAG;
		}
		if (dataCfg->counter < dataCfg->length){
			// Write data to buffer
			if(dataCfg->tx_data == NULL){
				if (spidat.dataword == 0){
					SPI_SendData(SPIx, 0xFF);
				} else {
					SPI_SendData(SPIx, 0xFFFF);
				}
			} else {
				if (spidat.dataword == 0){
					SPI_SendData(SPIx, (*(uint8_t *)dataCfg->tx_data));
				} else {
					SPI_SendData(SPIx, (*(uint16_t *)dataCfg->tx_data));
				}
			}
			SPI_IntCmd(SPIx, ENABLE);
		} else {
			// Save status
			dataCfg->status = SPI_STAT_DONE;
		}
		return (0);
	}
}


/********************************************************************//**
 * @brief 		Enable or disable SPIx interrupt.
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @param[in]	NewState New state of specified UART interrupt type,
 * 				should be:
 * 				- ENALBE: Enable this SPI interrupt.
* 				- DISALBE: Disable this SPI interrupt.
 * @return 		None
 *********************************************************************/
void SPI_IntCmd(LPC_SPI_TypeDef *SPIx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SPIx->SPCR |= SPI_SPCR_SPIE;
	}
	else
	{
		SPIx->SPCR &= (~SPI_SPCR_SPIE) & SPI_SPCR_BITMASK;
	}
}


/********************************************************************//**
 * @brief 		Checks whether the SPI interrupt flag is set or not.
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @return 		The new state of SPI Interrupt Flag (SET or RESET)
 *********************************************************************/
IntStatus SPI_GetIntStatus (LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return ((SPIx->SPINT & SPI_SPINT_INTFLAG) ? SET : RESET);
}


/********************************************************************//**
 * @brief 		Clear SPI interrupt flag.
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @return 		None
 *********************************************************************/
void SPI_ClearIntPending(LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	SPIx->SPINT = SPI_SPINT_INTFLAG;
}


/********************************************************************//**
 * @brief 		Get current value of SPI Status register in SPIx peripheral.
 * @param[in]	SPIx	SPI peripheral selected, should be SPI
 * @return		Current value of SPI Status register in SPI peripheral.
 * Note:	The return value of this function must be used with
 * 			SPI_CheckStatus() to determine current flag status
 * 			corresponding to each SPI status type. Because some flags in
 * 			SPI Status register will be cleared after reading, the next reading
 * 			SPI Status register could not be correct. So this function used to
 * 			read SPI status register in one time only, then the return value
 * 			used to check all flags.
 *********************************************************************/
uint32_t SPI_GetStatus(LPC_SPI_TypeDef* SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return (SPIx->SPSR & SPI_SPSR_BITMASK);
}



/********************************************************************//**
 * @brief 		Checks whether the specified SPI Status flag is set or not
 * 				via inputSPIStatus parameter.
 * @param[in]	inputSPIStatus Value to check status of each flag type.
 * 				This value is the return value from SPI_GetStatus().
 * @param[in]	SPIStatus	Specifies the SPI status flag to check,
 * 				should be one of the following:
				- SPI_STAT_ABRT: Slave abort.
				- SPI_STAT_MODF: Mode fault.
				- SPI_STAT_ROVR: Read overrun.
				- SPI_STAT_WCOL: Write collision.
				- SPI_STAT_SPIF: SPI transfer complete.
 * @return 		The new state of SPIStatus (SET or RESET)
 *********************************************************************/
FlagStatus SPI_CheckStatus (uint32_t inputSPIStatus,  uint8_t SPIStatus)
{
	CHECK_PARAM(PARAM_SPI_STAT(SPIStatus));

	return ((inputSPIStatus & SPIStatus) ? SET : RESET);
}

void SPI_IRQHandler(void)
{
    SPI_StdIntHandler();

}


/**
 * @brief		Standard SPI Interrupt handler
 * @param[in] 	None
 * @return		None
 */
//modify by pli
void SPI_StdIntHandler(void)
//void SPI_IRQHandler(void)
{
	// Call relevant handler
	spidat.inthandler();
}


void spi_read(LPC_SPI_TypeDef *SPIx, uint8_t *bufptr, uint32_t size, void (*rx_callback)(void))
{
    SPI_DATA_SETUP_Type *dataCfg;
    dataCfg = (SPI_DATA_SETUP_Type *)spidat.txrx_setup;
    rd_ongoing = 1;
    if(wr_ongoing == 0)
    {
        xferConfig.tx_data = NULL;
        xferConfig.rx_data = bufptr;
        xferConfig.length = size;
        xferConfig.rd_length = size;
        xferConfig.wr_length = size;
        xferConfig.callback = rx_callback;
        xferConfig.counter = 0;
        SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_INTERRUPT);
    }else 
    {
        dataCfg->rd_length = size;
        dataCfg->rx_data = xferConfig.rx_data;
        if(dataCfg->rd_length > (dataCfg->length - dataCfg->counter))
        {
           dataCfg->length = dataCfg->rd_length + dataCfg->counter;
        }
    }
}

/*
 ****************************************************************************************
 * @brief Starts a data transmission.
 *
 *****************************************************************************************
 */
void spi_write(LPC_SPI_TypeDef *SPIx, uint8_t *bufptr, uint32_t size, void (*tx_callback)(void))
{
    SPI_DATA_SETUP_Type *dataCfg;
    dataCfg = (SPI_DATA_SETUP_Type *)spidat.txrx_setup;
    wr_ongoing = 1;
    if(rd_ongoing == 0)
    {
        xferConfig.tx_data = tx_buffer;
        memcpy(xferConfig.tx_data,bufptr,size);
        xferConfig.rx_data = NULL;
        xferConfig.length = size;
        xferConfig.wr_length = size;
        xferConfig.rd_length = size;
        xferConfig.callback = tx_callback;
        xferConfig.counter = 0;
        SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_INTERRUPT);
    }else {
        dataCfg->tx_data =  xferConfig.tx_data;
        memcpy(xferConfig.tx_data,bufptr,size);
        dataCfg->wr_length = size;
        if(dataCfg->wr_length > (dataCfg->length - dataCfg->counter))
        {
            dataCfg->length = dataCfg->wr_length + dataCfg->counter;
        }
    }
}


/**
 * @}
 */

#endif /* _SPI */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
