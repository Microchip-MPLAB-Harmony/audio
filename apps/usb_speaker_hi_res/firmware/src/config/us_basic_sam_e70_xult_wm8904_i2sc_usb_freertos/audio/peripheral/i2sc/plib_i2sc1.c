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
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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
                            I2SC_MR_DATALENGTH(0x1) |	// Data Word Length
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

/*********************************************************************************
 * I2SC1_PLLAClockSet() - Master Mode PLLA Clock set for the I2SC
*********************************************************************************/
uint32_t I2SC1_PLLAClockSet(const uint8_t div, const uint8_t mul)
{
    if ( (div > 0) && (div < 257) &&
         (mul > 0) && (mul < 64) )
    {
        /* Configure and Enable PLLA */
        PMC_REGS->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk | 
                               CKGR_PLLAR_PLLACOUNT(0x3f) |
                               CKGR_PLLAR_MULA(mul - 1) |
                               CKGR_PLLAR_DIVA(div);

        while ( (PMC_REGS->PMC_SR & PMC_SR_LOCKA_Msk) != PMC_SR_LOCKA_Msk);
        return mul;
    }
    else
    {
        //Do Nothing
        return 0;
    }

}

/*********************************************************************************
 * I2SC1_GenericClockSet() - Master mode set GCLK for the I2SC 
*********************************************************************************/
uint32_t I2SC1_GenericClockSet(const uint8_t div2)
{
    //Range check
    if ( (div2 > 0) && (div2 < 257))
    {
        /* Setup Generic/Peripheral Clock for I2S1 */
        PMC_REGS->PMC_PCR =  PMC_PCR_PID(70) |    //See plib_clk.c 
                             PMC_PCR_CMD_Msk   | 
                             PMC_PCR_GCLKEN_Msk |  
                             PMC_PCR_EN_Msk | 
                             PMC_PCR_GCLKCSS_PLLA_CLK | 
                             PMC_PCR_GCLKDIV(div2-1);

        MATRIX_REGS->CCFG_PCCR |=  CCFG_PCCR_I2SC1CC_Msk ;

        return div2; 
    }
    else
    {
        return 0; 
    }
}

/*********************************************************************************
 * I2SC1_ProgrammableClockSet() - Set the Programmable Clock Divider for PCKx
 * 
 * NOTE:  This is only used for E70 xult board implementation of I2SC1 interfacejj
*********************************************************************************/
uint32_t I2SC1_ProgrammableClockSet(const uint8_t pClkNum, const uint8_t div2)
{

    if ((pClkNum > 0) && (pClkNum < 8))
    {
        //uint32_t bitPosDis     = _U_(8) + pClkNum;
        //uint32_t bitMaskPckDis = (_U_(0x1) << bitPosDis);                    
        //uint32_t bitPosEn      = _U_(8) + pClkNum;           
        //uint32_t bitMaskPckEn  = (_U_(0x1) << bitPosEn); 
        //uint32_t bitPosRdy     = _U_(8) + pClkNum;           
        //uint32_t bitMaskPckRdy = (_U_(0x1) << bitPosRdy); 

        //TODO: For now allow the glitch on the clock change.
        /* Disable selected programmable clock  */
        //PMC_REGS->PMC_SCDR = bitMaskPckDis;

        /* Configure selected programmable clock    */
        PMC_REGS->PMC_PCK[pClkNum]= PMC_PCK_CSS_PLLA_CLK | PMC_PCK_PRES(div2-1);

        /* Enable selected programmable clock   */
        //PMC_REGS->PMC_SCER =    bitMaskPckEn;

        /* Wait for clock to be ready   */
        //while( (PMC_REGS->PMC_SR & (bitMaskPckRdy) ) != (bitMaskPckRdy) );
        return div2;
    }
    else
    {
        return 0;
    }
}
/*******************************************************************************
 End of File
*/

