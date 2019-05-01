/*******************************************************************************
  SSC PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ssc.c

  Summary:
    SSC Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SSC peripheral.

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

#include "plib_ssc.h"

// *****************************************************************************
// *****************************************************************************
// Section: SSC Implementation
// *****************************************************************************
// *****************************************************************************

void SSC_Initialize ( void )
{
    /* Disable and Reset the SSC*/
    SSC_REGS->SSC_CR = SSC_CR_TXDIS_Msk | SSC_CR_RXDIS_Msk;

    SSC_REGS->SSC_CR = SSC_CR_SWRST_Msk;
             
    /* Receiver Configurations */
    SSC_REGS->SSC_RCMR =  SSC_RCMR_CKS(2) |
                           SSC_RCMR_CKO(0) |
                           SSC_RCMR_CKI(0) |
                           SSC_RCMR_CKG(0) |
                           SSC_RCMR_START(7) |
                           SSC_RCMR_STOP(0) | 
                           SSC_RCMR_STTDLY(1) |
                           SSC_RCMR_PERIOD(0) ;

    SSC_REGS->SSC_RFMR =  SSC_RFMR_DATLEN(15) |
                           SSC_RFMR_LOOP(0) |
                           SSC_RFMR_MSBF(1) |
                           SSC_RFMR_DATNB(0) |
                           SSC_RFMR_FSLEN(0) |
                           SSC_RFMR_FSOS(0) |
                           SSC_RFMR_FSEDGE(0) |
                           SSC_RFMR_FSLEN_EXT(0) ;
    
    /* Transmitter Configurations */

    SSC_REGS->SSC_TCMR =  SSC_TCMR_CKS(2) |
                           SSC_TCMR_CKO(0) |
                           SSC_TCMR_CKI(0) |
                           SSC_TCMR_CKG(0) |
                           SSC_TCMR_START(7) |
                           SSC_TCMR_STTDLY(1) |
                           SSC_TCMR_PERIOD(0) ;

    SSC_REGS->SSC_TFMR = SSC_TFMR_DATLEN(15) |
                           SSC_TFMR_DATDEF(0) |
                           SSC_TFMR_MSBF(1) |
                           SSC_TFMR_DATNB(0) |
                           SSC_TFMR_FSLEN(0) |
                           SSC_TFMR_FSOS(0) |
                           SSC_TFMR_FSDEN(0) |
                           SSC_TFMR_FSEDGE(0) |
                           SSC_TFMR_FSLEN_EXT(0) ;

    SSC_REGS->SSC_CMR = 0;       // not used when SSC is in slave mode

    SSC_REGS->SSC_CR = SSC_CR_TXEN_Msk | SSC_CR_RXEN_Msk;        
}

void SSC_BaudSet(const uint32_t baud)
{
    // not used when SSC is in slave mode
}

uint32_t SSC_LRCLK_Get(void)
{
    // for I2S format, will sync on low to high transition
    volatile uint32_t ret = ((PIOB_REGS->PIO_PDSR >> 0) & 0x1);
    return ret;    
}

/*******************************************************************************
 End of File
*/

