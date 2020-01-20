/*******************************************************************************
  TC Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_tc2.c

  Summary
    TC peripheral library source file.

  Description
    This file implements the interface to the TC peripheral library.  This
    library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file.
*/
#include "device.h"
#include "plib_tc2.h"

 
 

 

 
 

 

 

/* Callback object for channel 1 */
TC_COMPARE_CALLBACK_OBJECT TC2_CH1_CallbackObj;

/* Initialize channel in compare mode */
void TC2_CH1_CompareInitialize (void)
{
    /* Use peripheral clock */
    TC2_REGS->TC_CHANNEL[1].TC_EMR = TC_EMR_NODIVCLK_Msk;
    /* clock selection and waveform selection */
    TC2_REGS->TC_CHANNEL[1].TC_CMR =  TC_CMR_WAVEFORM_WAVSEL_UP_RC | TC_CMR_WAVE_Msk | \
                TC_CMR_WAVEFORM_ACPA_SET | TC_CMR_WAVEFORM_ACPC_CLEAR | TC_CMR_WAVEFORM_AEEVT_CLEAR\
           | TC_CMR_WAVEFORM_BCPB_SET | TC_CMR_WAVEFORM_BCPC_CLEAR | TC_CMR_WAVEFORM_BEEVT_CLEAR ;

    /* external reset event configurations */
    TC2_REGS->TC_CHANNEL[1].TC_CMR |= TC_CMR_WAVEFORM_ENETRG_Msk | TC_CMR_WAVEFORM_EEVT_XC0 | \
                TC_CMR_WAVEFORM_EEVTEDG_NONE;

    /* write period */
    TC2_REGS->TC_CHANNEL[1].TC_RC = 10000U;

    /* write compare values */
    TC2_REGS->TC_CHANNEL[1].TC_RA = 5000U;
    TC2_REGS->TC_CHANNEL[1].TC_RB = 3000U;

    /* enable interrupt */
    TC2_REGS->TC_CHANNEL[1].TC_IER = TC_IER_CPCS_Msk;
    TC2_CH1_CallbackObj.callback_fn = NULL;
}

/* Start the compare mode */
void TC2_CH1_CompareStart (void)
{
    TC2_REGS->TC_CHANNEL[1].TC_CCR = (TC_CCR_CLKEN_Msk | TC_CCR_SWTRG_Msk);
}

/* Stop the compare mode */
void TC2_CH1_CompareStop (void)
{
    TC2_REGS->TC_CHANNEL[1].TC_CCR = (TC_CCR_CLKDIS_Msk);
}

uint32_t TC2_CH1_CompareFrequencyGet( void )
{
    return (uint32_t)(150000000UL);
}

/* Configure the period value */
void TC2_CH1_ComparePeriodSet (uint16_t period)
{
    TC2_REGS->TC_CHANNEL[1].TC_RC = period;
}

/* Read the period value */
uint16_t TC2_CH1_ComparePeriodGet (void)
{
    return TC2_REGS->TC_CHANNEL[1].TC_RC;
}

/* Set the compare A value */
void TC2_CH1_CompareASet (uint16_t value)
{
    TC2_REGS->TC_CHANNEL[1].TC_RA = value;
}

/* Set the compare B value */
void TC2_CH1_CompareBSet (uint16_t value)
{
    TC2_REGS->TC_CHANNEL[1].TC_RB = value;
}

/* Register callback function */
void TC2_CH1_CompareCallbackRegister(TC_COMPARE_CALLBACK callback, uintptr_t context)
{
    TC2_CH1_CallbackObj.callback_fn = callback;
    TC2_CH1_CallbackObj.context = context;
}

/* Interrupt handler for Channel 1 */
void TC2_CH1_InterruptHandler(void)
{
    TC_COMPARE_STATUS compare_status = (TC_COMPARE_STATUS)(TC2_REGS->TC_CHANNEL[1].TC_SR & TC_COMPARE_STATUS_MSK);
    /* Call registered callback function */
    if ((TC_COMPARE_NONE != compare_status) && TC2_CH1_CallbackObj.callback_fn != NULL)
    {
        TC2_CH1_CallbackObj.callback_fn(compare_status, TC2_CH1_CallbackObj.context);
    }
}
 

 
 

 

 
/**
 End of File
*/
