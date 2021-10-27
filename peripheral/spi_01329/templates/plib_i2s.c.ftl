/*******************************************************************************
  SPI-I2S PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_${I2S_INSTANCE_NAME?lower_case}.c

  Summary:
    ${I2S_INSTANCE_NAME} Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI-I2S peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>                    // Defines true
#include "plib_${I2S_INSTANCE_NAME?lower_case}.h"

// *****************************************************************************
// *****************************************************************************
// Section: ${I2S_INSTANCE_NAME} Implementation
// *****************************************************************************
// *****************************************************************************

#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))

#define ${SPI_INSTANCE_NAME}_CON_FRMEN                      (1 << _${SPI_INSTANCE_NAME}CON_FRMEN_POSITION)      // always 1 for I2S
#define ${SPI_INSTANCE_NAME}_CON_FRMSYNC                    (${I2S_SPICON_FRMSYNC} << _${SPI_INSTANCE_NAME}CON_FRMSYNC_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_FRMPOL                     (${I2S_SPICON_FRMPOL} << _${SPI_INSTANCE_NAME}CON_FRMPOL_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_MSSEN                      (1 << _${SPI_INSTANCE_NAME}CON_MSSEN_POSITION)      // always 1 for I2S
#define ${SPI_INSTANCE_NAME}_CON_FRMSYPW                    (0 << _${SPI_INSTANCE_NAME}CON_FRMSYPW_POSITION)    // always 0 for I2S
#define ${SPI_INSTANCE_NAME}_CON_FRMCNT                     (${I2S_SPICON_FRMCNT} << _${SPI_INSTANCE_NAME}CON_FRMCNT_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_MCLKSEL                    (1 << _${SPI_INSTANCE_NAME}CON_MCLKSEL_POSITION)    // always 1 for I2S
#define ${SPI_INSTANCE_NAME}_CON_SPIFE                      (${I2S_SPICON_SPIFE} << _${SPI_INSTANCE_NAME}CON_SPIFE_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_ENHBUF                     (0 << _${SPI_INSTANCE_NAME}CON_ENHBUF_POSITION)     // always 0 for I2S
#define ${SPI_INSTANCE_NAME}_CON_MODE_32_MODE_16            (${I2S_SPICON_MODE} << _${SPI_INSTANCE_NAME}CON_MODE16_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_CKE                        (${I2S_SPICON_CLK_PH} << _${SPI_INSTANCE_NAME}CON_CKE_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_CKP                        (${I2S_SPICON_CLK_POL} << _${SPI_INSTANCE_NAME}CON_CKP_POSITION)
#define ${SPI_INSTANCE_NAME}_CON_MSTEN                      (${I2S_MSTR_MODE_EN} << _${SPI_INSTANCE_NAME}CON_MSTEN_POSITION)

#define ${SPI_INSTANCE_NAME}_CON2_IGNROV                    (1 << _${SPI_INSTANCE_NAME}CON2_IGNROV_POSITION)
#define ${SPI_INSTANCE_NAME}_CON2_IGNTUR                    (1 << _${SPI_INSTANCE_NAME}CON2_IGNTUR_POSITION)
#define ${SPI_INSTANCE_NAME}_CON2_AUDEN                     (1 << _${SPI_INSTANCE_NAME}CON2_AUDEN_POSITION)     // 1 for I2S mode
#define ${SPI_INSTANCE_NAME}_CON2_AUDMONO                   (${I2S_SPICON2_MONO} << _${SPI_INSTANCE_NAME}CON2_AUDMONO_POSITION)
#define ${SPI_INSTANCE_NAME}_CON2_AUDMOD                    (${I2S_SPICON2_MODE} << _${SPI_INSTANCE_NAME}CON2_AUDMOD_POSITION)

void ${I2S_INSTANCE_NAME}_Initialize ( void )
{
    uint32_t rdata;

    /* Disable ${SPI_INSTANCE_NAME} Interrupts */
    ${SPI_FLT_IEC_REG}CLR = ${SPI_FLT_IEC_REG_MASK};
    ${SPI_RX_IEC_REG}CLR = ${SPI_RX_IEC_REG_MASK};
    ${SPI_TX_IEC_REG}CLR = ${SPI_TX_IEC_REG_MASK};

    /* STOP and Reset the SPI */
    ${SPI_INSTANCE_NAME}CON = 0;

    /* Clear the Receiver buffer */
    rdata = ${SPI_INSTANCE_NAME}BUF;
    rdata = rdata;

    /* Clear ${SPI_INSTANCE_NAME} Interrupt flags */
    ${SPI_FLT_IFS_REG}CLR = ${SPI_FLT_IFS_REG_MASK};
    ${SPI_RX_IFS_REG}CLR = ${SPI_RX_IFS_REG_MASK};
    ${SPI_TX_IFS_REG}CLR = ${SPI_TX_IFS_REG_MASK};

    /* BAUD Rate register Setup */
    ${SPI_INSTANCE_NAME}BRG = ${I2S_BRG_VALUE};

    /* CLear the Overflow */
    ${SPI_INSTANCE_NAME}STATCLR = _${SPI_INSTANCE_NAME}STAT_SPIROV_MASK;

    ${SPI_INSTANCE_NAME}CONSET = ${SPI_INSTANCE_NAME}_CON_MSSEN | ${SPI_INSTANCE_NAME}_CON_MCLKSEL | ${SPI_INSTANCE_NAME}_CON_ENHBUF | ${SPI_INSTANCE_NAME}_CON_MODE_32_MODE_16 | ${SPI_INSTANCE_NAME}_CON_CKE | ${SPI_INSTANCE_NAME}_CON_CKP |
       ${SPI_INSTANCE_NAME}_CON_MSTEN | ${SPI_INSTANCE_NAME}_CON_FRMEN | ${SPI_INSTANCE_NAME}_CON_FRMPOL | ${SPI_INSTANCE_NAME}_CON_FRMSYNC | ${SPI_INSTANCE_NAME}_CON_FRMSYPW | ${SPI_INSTANCE_NAME}_CON_FRMCNT | ${SPI_INSTANCE_NAME}_CON_SPIFE;

    ${SPI_INSTANCE_NAME}CON2SET = ${SPI_INSTANCE_NAME}_CON2_IGNROV | ${SPI_INSTANCE_NAME}_CON2_IGNTUR | ${SPI_INSTANCE_NAME}_CON2_AUDEN | ${SPI_INSTANCE_NAME}_CON2_AUDMONO | ${SPI_INSTANCE_NAME}_CON2_AUDMOD;

    /* Enable ${SPI_INSTANCE_NAME} */
    ${SPI_INSTANCE_NAME}CONSET = _${SPI_INSTANCE_NAME}CON_ON_MASK;
}

<#if I2S_LRCLK_INVERT== "1">
uint32_t ${I2S_INSTANCE_NAME}_LRCLK_Get(void)
{
    // for inverted (e.g. left-justified format), will sync on high to low transition
    volatile uint32_t ret = 1-${I2S_LRCLK_PIN_DEFINE};
    return ret;
}
<#else>
uint32_t ${I2S_INSTANCE_NAME}_LRCLK_Get(void)
{
    // for I2S format, will sync on low to high transition
    volatile uint32_t ret = ${I2S_LRCLK_PIN_DEFINE};
    return ret;
}
</#if>

static uint32_t _calcRefclock(uint32_t sysclk, uint32_t rodivInt, uint32_t rotrimInt)
{
    return sysclk / ((uint32_t)2.0*((double)rodivInt+(double)rotrimInt/512.0));    
}

uint32_t ${I2S_INSTANCE_NAME}_RefClockSet(uint32_t sysclk, uint32_t samplingRate, uint32_t mclk_sampleRate_multiplier)
{   
    // e.g. target sysclk = 198 MHz, samplingRate == 44100, mclk_sampleRate_multiplier = 256, so ideal refclk = 11289600
 
    // refclk = sysclk / (2 * (rodiv + (rotrim / 512) )
    // closest fit: rodiv = 8, rotrim = 394:  198000000 / (2 * (8 + 394/512) ) => 11289086,  /256 = 44097
    
    uint32_t refclk = samplingRate * mclk_sampleRate_multiplier;      // target ref clock
    
    // rodiv = sysclk / (2* refclk) - (rotrim / 512)
    double rodiv = (double)sysclk / (2.0*(double)refclk);
    uint16_t rodivInt = (uint16_t)rodiv;                // integer part becomes rodiv
    
    double rotrim = 512.0*(rodiv - (double)rodivInt);   // fractional part is rotrim/512      
    uint16_t rotrimInt = (uint16_t)rotrim;
     
    uint32_t refclk_m1 = (rotrimInt > 0) ? _calcRefclock(sysclk, rodivInt, rotrimInt-1) : _calcRefclock(sysclk, rodivInt-1, 511);       
    uint32_t refclk_0 = _calcRefclock(sysclk, rodivInt, rotrimInt);    
    uint32_t refclk_p1 = _calcRefclock(sysclk, rodivInt, rotrimInt+1);
    
    uint32_t error_m1 = abs(refclk_m1 - refclk);
    uint32_t error_0 = abs(refclk_0 - refclk);
    uint32_t error_p1 = abs(refclk_p1 - refclk);
    
    uint32_t minError = MIN(error_m1,error_0);
    minError = MIN(minError,error_p1);
    
    if (minError == error_m1)
    {
        if (rotrimInt > 0)
        {
            rotrimInt--;
        }
        else
        {
            rotrimInt = 511;
            rodivInt--;
        }
    }
    else if (minError == error_p1)
    {
        rotrimInt++;
    }
    // else rodivInt, rotrimInt ok as is
 
    // for debug
    //printf("%s_RefClockSet: %d %f %d %f %d %d %d %d\r\n","${I2S_INSTANCE_NAME}",
    //        refclock,rodiv,rodivInt,rotrim,rotrimInt,refclock_m1,refclock_0,refclock_p1);
    //printf("                  %d %d %d %d\r\n",error_m1,error_0,error_p1,minError);  
    
    uint32_t refclko = _calcRefclock(sysclk, rodivInt, rotrimInt); 
    uint32_t calcSamplingrate = refclko / mclk_sampleRate_multiplier;
    
    printf("%s_RefClockSet: %d %d %d %d\r\n","${I2S_INSTANCE_NAME}",
            rodivInt,rotrimInt,calcSamplingrate,refclko);
    
    unsigned int int_flag = false;
    int_flag = __builtin_disable_interrupts();

    /* unlock system for clock configuration */
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    if (int_flag)
    {
        __builtin_mtc0(12, 0,(__builtin_mfc0(12, 0) | 0x0001)); /* enable interrupts */
    }

<#if __PROCESSOR?matches("PIC32MZ.*") == true>
    uint32_t refclkConOld = REFO1CON;
    REFO1CON = refclkConOld & 7;       // On bit 0, but keep rosel field
    while (REFO1CON & 1);   // wait for Active bit to be 0
    
    REFO1CONSET = rodivInt*0x10000 | 0x200; // replace with new value    
    REFO1TRIM = rotrimInt*0x800000;
    
    REFO1CONSET = 0x9000;      // On bit, OE and Divswen = 1
<#elseif __PROCESSOR?matches("PIC32MX.*") == true>
    uint32_t refclkConOld = REFOCON;
    REFOCON = refclkConOld & 7;       // On bit 0, but keep rosel field
    while (REFOCON & 1);   // wait for Active bit to be 0
    
    REFOCONSET = rodivInt*0x10000 | 0x200; // replace with new value    
    REFOTRIM = rotrimInt*0x800000;
    
    REFOCONSET = 0x9000;      // On bit, OE and Divswen = 1
</#if>
   
    /* Lock system since done with clock configuration */
    int_flag = __builtin_disable_interrupts();

    SYSKEY = 0x33333333;

    if (int_flag) /* if interrupts originally were enabled, re-enable them */
    {
        __builtin_mtc0(12, 0,(__builtin_mfc0(12, 0) | 0x0001));
    }
    
    // for debug
<#if __PROCESSOR?matches("PIC32MZ.*") == true>
    printf("                  %08x %08x\r\n",REFO1CON,REFO1TRIM);   
<#elseif __PROCESSOR?matches("PIC32MX.*") == true>
    printf("                  %08x %08x\r\n",REFOCON,REFOTRIM);
</#if>       
    return calcSamplingrate;
}

uint32_t ${I2S_INSTANCE_NAME}_BaudRateSet(uint32_t bitClk, uint32_t baudRate)
{
    // bitClk is the bit clock, typically 4 * sampling rate
    // BRG = (bitClk/(2 * baudRate)) - 1 
    uint32_t t_brg;
    uint32_t baudHigh;
    uint32_t baudLow;
    uint32_t errorHigh;
    uint32_t errorLow;
    
    t_brg = (((bitClk/baudRate)/2u) - 1u);
    
    baudHigh = bitClk / (2u * (t_brg + 1u));
    baudLow = bitClk / (2u * (t_brg + 2u));
    errorHigh = baudHigh - baudRate;
    errorLow = baudRate - baudLow;

    if (errorHigh > errorLow)
    {
        t_brg++;
    }
    
    printf("%s_BaudRateSet: %d %d\r\n","${I2S_INSTANCE_NAME}",bitClk,t_brg);    
    ${SPI_INSTANCE_NAME}BRG = t_brg;
    
    return t_brg;
}
 
