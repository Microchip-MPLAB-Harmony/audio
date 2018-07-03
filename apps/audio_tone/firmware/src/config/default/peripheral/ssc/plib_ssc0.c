/*******************************************************************************
  SSC PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ssc0.c

  Summary:
    SSC0 Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SSC peripheral.

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2018 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/

#include "plib_ssc0.h"

// *****************************************************************************
// *****************************************************************************
// Section: SSC0 Implementation
// *****************************************************************************
// *****************************************************************************

// Since there is only one SSC module, the registers are accessed using SSC_REGS
// instead of SSCn->REGS, where n is the instance number

void SSC0_Initialize ( void )
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

    SSC_REGS->SSC_CMR = 0x0;       // not used when SSC is in slave mode

    SSC_REGS->SSC_CR = SSC_CR_TXEN_Msk | SSC_CR_RXEN_Msk;        
}

void SSC0_BaudSet(const uint32_t baud)
{
    // not used when SSC is in slave mode
}

/*******************************************************************************
 End of File
*/

