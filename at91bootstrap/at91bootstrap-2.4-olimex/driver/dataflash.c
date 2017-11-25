/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 * File Name           : dataflash.c
 * Object              : ATMEL DataFlash High level functions
 * Creation            : NLe Jul 12th 2006
 *---------------------------------------------------------------------------
*/

#include "../include/spi.h"
#include "../include/part.h"
#include "../include/main.h"
#include "../include/dataflash.h"
#include "../include/debug.h"
#include <stdlib.h>

#define	SUCCESS	1
#define	FAILURE	0

#ifdef CFG_DATAFLASH
extern	div_t udiv(unsigned int numerator, unsigned int denominator);
/* Write SPI register */
static inline void write_spi(unsigned int offset, const unsigned int value)
{
	writel(value, offset + AT91C_BASE_SPI);
}

/* Read SPI registers */
static inline unsigned int read_spi(unsigned int offset)
{
	return readl(offset + AT91C_BASE_SPI);
}

/*------------------------------------------------------------------------------*/
/* \fn    df_spi_init								*/
/* \brief Configure SPI								*/
/*------------------------------------------------------------------------------*/
static int df_spi_init(unsigned int pcs, unsigned int spi_csr)
{
	unsigned int ncs = 0;

	/* Open PIO for SPI0 */
	df_hw_init();

	/* Enables the SPI0 Clock */
	writel((1 << AT91C_ID_SPI), PMC_PCER + AT91C_BASE_PMC);

	/* Reset SPI0 */
	write_spi(SPI_CR, AT91C_SPI_SWRST);

    	/* Configure SPI0 in Master Mode with No CS selected */
    	write_spi(SPI_MR, AT91C_SPI_MSTR | AT91C_SPI_MODFDIS | AT91C_SPI_PCS);

	switch (pcs)
	{
		case AT91C_SPI_PCS0_DATAFLASH:	ncs = 0;	break;
		case AT91C_SPI_PCS1_DATAFLASH:	ncs = 1;	break;
		case AT91C_SPI_PCS2_DATAFLASH:	ncs = 2;	break;
		case AT91C_SPI_PCS3_DATAFLASH:	ncs = 3;	break;
	}
	/* Configure CSx */
	write_spi(SPI_CSR + 4*ncs, spi_csr);

	/* Choose CSx */
	write_spi(SPI_MR, read_spi(SPI_MR) & 0xFFF0FFFF);
	write_spi(SPI_MR, read_spi(SPI_MR) | ((pcs<<16) & AT91C_SPI_PCS));

	/* SPI_Enable */
	write_spi(SPI_CR, AT91C_SPI_SPIEN);

	return SUCCESS;
}

/*------------------------------------------------------------------------------*/
/* \fn    df_is_busy								*/
/* \brief Test if SPI has received a buffer or not				*/
/*------------------------------------------------------------------------------*/
static AT91S_DF_SEM df_is_busy(
	AT91PS_DF pDataFlash)
{
	unsigned int dStatus = read_spi(SPI_SR);

	/* If End of Receive Transfer interrupt occurred */
 	if (( dStatus & AT91C_SPI_RXBUFF))
 	{
	 	write_spi(SPI_PTCR, AT91C_PDC_TXTDIS);	/* PDC Disable Tx */
	 	write_spi(SPI_PTCR, AT91C_PDC_RXTDIS);	/* PDC Disable Rx */

 		/* Release the semaphore */
		pDataFlash->bSemaphore = UNLOCKED;
		return UNLOCKED;
	}
	return  pDataFlash->bSemaphore;
}

/*------------------------------------------------------------------------------*/
/* \fn    df_send_command							*/
/* \brief Generic function to send a command to the dataflash			*/
/*------------------------------------------------------------------------------*/
char df_send_command (
	AT91PS_DF pDataFlash,
	unsigned char bCmd,      /* Command value */
	unsigned char bCmdSize,  /* Command Size */
	char         *pData,     /* Data to be sent */
	unsigned int  dDataSize, /* Data Size */
	unsigned int  dAddress)  /* Dataflash Address */
{
	unsigned int dInternalAdr;
 	div_t result = udiv(dAddress, AT91C_PAGE_SIZE(pDataFlash));

 	/* Try to get the dataflash semaphore */
	if ((pDataFlash->bSemaphore) != UNLOCKED)
		return (char) FAILURE;
	pDataFlash->bSemaphore = LOCKED;
	/* Compute command pattern */
	dInternalAdr = (result.quot << AT91C_PAGE_OFFSET(pDataFlash)) + result.rem;
 	if (AT91C_DF_NB_PAGE(pDataFlash) >= 16384)
	{
		pDataFlash->command[0] = (bCmd & 0x000000FF) | \
	                             ((dInternalAdr & 0x0F000000) >> 16) | \
	                             ((dInternalAdr & 0x00FF0000) >>  0) | \
	                             ((dInternalAdr & 0x0000FF00) << 16);
 		pDataFlash->command[1] =  (dInternalAdr & 0x000000FF);

		if ((bCmd != DB_CONTINUOUS_ARRAY_READ) && (bCmd != DB_PAGE_READ))
			bCmdSize++;
	}
	else
	{
		pDataFlash->command[0] = (bCmd & 0x000000FF) | \
	                             ((dInternalAdr & 0x00FF0000) >> 8) | \
	                             ((dInternalAdr & 0x0000FF00) << 8) | \
	                             ((dInternalAdr & 0x000000FF) << 24);
 		pDataFlash->command[1] = 0;
	}

 	/* Send Command and data through the SPI */
 	write_spi(SPI_PTCR, AT91C_PDC_RXTDIS);				/* PDC Disable Rx*/
 	write_spi(SPI_RPR, (unsigned int) &(pDataFlash->command));	/* PDC Set Rx */
	write_spi(SPI_RCR, bCmdSize);
 	write_spi(SPI_RNPR, (unsigned int) pData);			/* PDC Set Next	Rx */
	write_spi(SPI_RNCR, dDataSize);

 	write_spi(SPI_PTCR, AT91C_PDC_TXTDIS);				/* PDC Disable Tx */
 	write_spi(SPI_TPR, (unsigned int) &(pDataFlash->command));	/* PDC Set Tx */
	write_spi(SPI_TCR, bCmdSize);
 	write_spi(SPI_TNPR, (unsigned int) pData);			/* PDC Set Next Tx */
	write_spi(SPI_TNCR, dDataSize);

 	write_spi(SPI_PTCR, AT91C_PDC_RXTEN);				/* PDC Enable Rx */
 	write_spi(SPI_PTCR, AT91C_PDC_TXTEN);				/* PDC Enable Tx */

    	while (df_is_busy(pDataFlash) == LOCKED);

	return SUCCESS;
}

/*------------------------------------------------------------------------------*/
/* \fn    df_wait_ready								*/
/* \brief wait for DataFlash to be ready					*/
/*------------------------------------------------------------------------------*/
static char df_wait_ready(AT91PS_DF pDataFlash)
{
	unsigned int timeout = 0;

	while (timeout++ < AT91C_DF_TIMEOUT)
	{
		if (df_get_status(pDataFlash))
		{
			if (df_is_ready(pDataFlash))
				return SUCCESS;
		}
	}

	return FAILURE;
}
volatile	int loop;
/*------------------------------------------------------------------------------*/
/* \fn    df_read								*/
/* \brief Read a block in dataflash						*/
/*------------------------------------------------------------------------------*/
static int df_read(
	AT91PS_DF pDf,
	unsigned int addr,
	unsigned char *buffer,
	unsigned int size)
{
	unsigned int SizeToRead;
	int page_counter;

	page_counter = 32;
	while (size)
	{
		SizeToRead = (size < 0x8000)? size : 0x8000;
		/* wait the dataflash ready status */
		if(df_wait_ready(pDf) != 0) {
		    	df_continuous_read(pDf, (char *)buffer, SizeToRead, addr);
			dbg_print(".");
			if(--page_counter <= 0) {
				page_counter = 32;
				dbg_print("\r\n");
			}
			size -= SizeToRead;
			addr += SizeToRead;
			buffer += SizeToRead;
		} else {
			/* We got a timeout */
			dbg_print("Timeout while waiting for dataflash ready\n");
			return FAILURE;
		}
	}
   	return SUCCESS;
}

/*----------------------------------------------------------------------*/
/* \fn    df_download							*/
/* \brief load the content of the dataflash				*/
/*----------------------------------------------------------------------*/
static int df_download(AT91PS_DF pDf, unsigned int img_addr, unsigned int img_size, unsigned int img_dest)
{
	dbg_print(">Loading from Dataflash[");
	dbg_print_hex(img_addr);
	dbg_print("] to SDRAM[");
	dbg_print_hex(img_dest);
	dbg_print("]\r\n");
	/* read bytes in the dataflash */
	if(df_read(pDf, img_addr,(unsigned char *)img_dest, img_size) == FAILURE)
		return FAILURE;
	dbg_print("\r\n>Loading complete, [");
	dbg_print_hex(IMG_SIZE);
	dbg_print("] bytes\r\n");
	/* wait the dataflash ready status */
	return df_wait_ready(pDf);
}

/*----------------------------------------------------------------------*/
/* \fn    df_probe							*/
/* \brief Returns DataFlash ID						*/
/*----------------------------------------------------------------------*/
static int df_probe(AT91PS_DF pDf)
{
    char *pResult = (char *)(pDf->command);

    df_get_status(pDf);
    return (pResult[1] & 0x3C);
}

/*----------------------------------------------------------------------*/
/* \fn    df_init							*/
/* \brief This function tries to identify the DataFlash connected	*/
/*----------------------------------------------------------------------*/
static int df_init (AT91PS_DF pDf)
{
	int dfcode = 0;
	int status = SUCCESS;

	/* Default: AT45DB321B */
	pDf->dfDescription.pages_number = 8192;
	pDf->dfDescription.pages_size = 528;
	pDf->dfDescription.page_offset = 10;

	dfcode = df_probe (pDf);

	switch (dfcode)
	{
/*		case AT45DB011B:
			pDf->dfDescription.pages_number = 512;
			pDf->dfDescription.pages_size = 264;
			pDf->dfDescription.page_offset = 9;
			break;

		case AT45DB021B:
			pDf->dfDescription.pages_number = 1024;
			pDf->dfDescription.pages_size = 264;
			pDf->dfDescription.page_offset = 9;
			break;

		case AT45DB041B:
			pDf->dfDescription.pages_number = 2048;
			pDf->dfDescription.pages_size = 264;
			pDf->dfDescription.page_offset = 9;
			break;

		case AT45DB081B:
			pDf->dfDescription.pages_number = 4096;
			pDf->dfDescription.pages_size = 264;
			pDf->dfDescription.page_offset = 9;
			break;
*/
		case AT45DB161B:
			pDf->dfDescription.pages_number = 4096;
			pDf->dfDescription.pages_size = 528;
			pDf->dfDescription.page_offset = 10;
			dbg_print(">AT45DB161D detected\r\n");
			break;

		case AT45DB321B:
			pDf->dfDescription.pages_number = 8192;
			pDf->dfDescription.pages_size = 528;
			pDf->dfDescription.page_offset = 10;
			dbg_print(">AT45DB321D detected\r\n");
			break;

		case AT45DB642:
			pDf->dfDescription.pages_number = 8192;
			pDf->dfDescription.pages_size = 1056;
			pDf->dfDescription.page_offset = 11;
			dbg_print(">AT45DB642D detected\r\n");
			break;
/*
		case AT45DB1282:
			pDf->dfDescription.pages_number = 16384;
			pDf->dfDescription.pages_size = 1056;
			pDf->dfDescription.page_offset = 11;
			break;

		case AT45DB2562:
			pDf->dfDescription.pages_number = 16384;
			pDf->dfDescription.pages_size = 2112;
			pDf->dfDescription.page_offset = 12;
			break;

		case AT45DB5122:
			pDf->dfDescription.pages_number = 32768;
			pDf->dfDescription.pages_size = 2112;
			pDf->dfDescription.page_offset = 12;
			break;
*/
		default:
		        status = FAILURE;
			break;
	}

	return status;
}

/*------------------------------------------------------------------------------*/
/* \fn    df_is_boot_valid							*/
/* \brief Check that the first bytes of the buffer are valid ARM vectors	*/
/*------------------------------------------------------------------------------*/
static unsigned int df_is_boot_valid(unsigned char *buffer)
{
	int i = 3;

 	/* Verify if the 28 first bytes of the sram correspond to ARM vectors
	   The sixth ARM vector contain the size of the code */
    	while(i < 28)
    	{
		if (i != 23)
		{
			if ((buffer[i] != 0xEA) && (buffer[i] != 0xE5) )
				return FAILURE;
		}
		i+=4;
    	}

    	return SUCCESS;
}

/*------------------------------------------------------------------------------*/
/* \fn    load_df								*/
/* \brief This function loads dataflash content to specified address		*/
/*------------------------------------------------------------------------------*/	
int load_df(unsigned int pcs, unsigned int img_addr, unsigned int img_size, unsigned int img_dest)
{
    	AT91S_DF sDF;
    	AT91PS_DF pDf = (AT91PS_DF)&sDF;
    	unsigned int rxBuffer[128];
	unsigned int status;

    	pDf->bSemaphore = UNLOCKED;

    	df_spi_init(pcs, DF_CS_SETTINGS);

    	if (df_init(pDf) == FAILURE)
        	return FAILURE;

#ifdef AT91SAM9260
	/* Test if a button has been pressed or not */
	/* Erase Page 0 to avoid infinite loop */
	df_recovery(pDf);
#endif

    	df_continuous_read(pDf, (char *)rxBuffer, 32, img_addr);
	df_wait_ready(pDf);

	if (df_is_boot_valid((unsigned char*)rxBuffer) == FAILURE) {
		dbg_print(">Invalid Boot Area...\n\r");
		return FAILURE;
	}

	status = df_download(pDf, img_addr, img_size, img_dest);
	return status;
}

#endif /* CFG_DATAFLASH */
