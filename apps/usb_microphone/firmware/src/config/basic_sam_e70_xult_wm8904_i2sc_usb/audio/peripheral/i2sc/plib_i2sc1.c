/*******************************************************************************
  I2SC PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_i2sc1.c

  Summary:
    I2SC1 Source File

  Description:
    This file has the implementation of all the interfaces provided for one
    particular instance of the Inter-IC Sound Controller (I2SC) peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "plib_i2sc1.h"

// *****************************************************************************
// *****************************************************************************
// Section: I2SC1 Implementation
// *****************************************************************************
// *****************************************************************************

void I2SC1_Initialize ( void )
{
    // Disable and reset the I2SC
    I2SC1_REGS->I2SC_CR = I2SC_CR_TXDIS_Msk | I2SC_CR_RXDIS_Msk | I2SC_CR_CKDIS_Msk;
    I2SC1_REGS->I2SC_CR = I2SC_CR_SWRST_Msk;
       
    I2SC1_REGS->I2SC_MR = I2SC_MR_MODE(1) |		// Master/Slave Mode		
                            I2SC_MR_DATALENGTH(0x4) |	// Data Word Length
                            I2SC_MR_RXMONO(0) |		// Receiver Mono/Stereo
                            I2SC_MR_RXDMA(0) |		// # of DMA Channels for Receiver
                            I2SC_MR_RXLOOP(0) | 	// Loopback Test Mode
                            I2SC_MR_TXMONO(0) |		// Transmitter Mono/Stereo
                            I2SC_MR_TXDMA(0) |		// # of DMA Channels for Transmitter
                            I2SC_MR_TXSAME(0) |		// Transmit Data When Underrun
                            I2SC_MR_IMCKDIV(1) |	// Selected Clock to IMCK Ratio
                            I2SC_MR_IMCKFS(0x7) |		// Master Clock to Sample Rate Ratio
                            I2SC_MR_IMCKMODE(1) |	// Master Clock Mode
                            I2SC_MR_IWS(0);		// Slot Width
    
    // Enable the I2SC
    I2SC1_REGS->I2SC_CR = I2SC_CR_TXEN_Msk | I2SC_CR_RXEN_Msk | I2SC_CR_CKEN_Msk;
    
    while(!(I2SC1_REGS->I2SC_SR & I2SC_SR_TXEN_Msk));
}

uint32_t I2SC1_LRCLK_Get(void)
{
    // for I2S format, will sync on low to high transition
    volatile uint32_t ret = ((PIOE_REGS->PIO_PDSR >> 0) & 0x1);
    return ret;    
}

/*******************************************************************************
 End of File
*/

