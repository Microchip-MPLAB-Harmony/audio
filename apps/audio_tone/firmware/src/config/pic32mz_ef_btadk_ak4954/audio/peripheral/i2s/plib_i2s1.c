/*******************************************************************************
  SPI-I2S PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_i2s1.c

  Summary:
    I2S1 Source File

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

#include "plib_i2s1.h"

// *****************************************************************************
// *****************************************************************************
// Section: I2S1 Implementation
// *****************************************************************************
// *****************************************************************************

#define SPI1_CON_FRMEN                      (1 << _SPI1CON_FRMEN_POSITION)      // always 1 for I2S
#define SPI1_CON_FRMSYNC                    (0 << _SPI1CON_FRMSYNC_POSITION)
#define SPI1_CON_FRMPOL                     (1 << _SPI1CON_FRMPOL_POSITION)
#define SPI1_CON_MSSEN                      (1 << _SPI1CON_MSSEN_POSITION)      // always 1 for I2S
#define SPI1_CON_FRMSYPW                    (0 << _SPI1CON_FRMSYPW_POSITION)    // always 0 for I2S
#define SPI1_CON_FRMCNT                     (4 << _SPI1CON_FRMCNT_POSITION)
#define SPI1_CON_MCLKSEL                    (1 << _SPI1CON_MCLKSEL_POSITION)    // always 1 for I2S
#define SPI1_CON_SPIFE                      (0 << _SPI1CON_SPIFE_POSITION)
#define SPI1_CON_ENHBUF                     (0 << _SPI1CON_ENHBUF_POSITION)     // always 0 for I2S
#define SPI1_CON_MODE_32_MODE_16            (1 << _SPI1CON_MODE16_POSITION)
#define SPI1_CON_CKE                        (1 << _SPI1CON_CKE_POSITION)
#define SPI1_CON_CKP                        (1 << _SPI1CON_CKP_POSITION)
#define SPI1_CON_MSTEN                      (1 << _SPI1CON_MSTEN_POSITION)

#define SPI1_CON2_IGNROV                    (1 << _SPI1CON2_IGNROV_POSITION)
#define SPI1_CON2_IGNTUR                    (1 << _SPI1CON2_IGNTUR_POSITION)
#define SPI1_CON2_AUDEN                     (1 << _SPI1CON2_AUDEN_POSITION)     // 1 for I2S mode
#define SPI1_CON2_AUDMONO                   (0 << _SPI1CON2_AUDMONO_POSITION)
#define SPI1_CON2_AUDMOD                    (0 << _SPI1CON2_AUDMOD_POSITION)

void I2S1_Initialize ( void )
{
    uint32_t rdata;

    /* Disable SPI1 Interrupts */
    IEC3CLR = 0x2000;
    IEC3CLR = 0x4000;
    IEC3CLR = 0x8000;

    /* STOP and Reset the SPI */
    SPI1CON = 0;

    /* Clear the Receiver buffer */
    rdata = SPI1BUF;
    rdata = rdata;

    /* Clear SPI1 Interrupt flags */
    IFS3CLR = 0x2000;
    IFS3CLR = 0x4000;
    IFS3CLR = 0x8000;

    /* BAUD Rate register Setup */
    SPI1BRG = 1;

    /* CLear the Overflow */
    SPI1STATCLR = _SPI1STAT_SPIROV_MASK;

    SPI1CONSET = SPI1_CON_MSSEN | SPI1_CON_MCLKSEL | SPI1_CON_ENHBUF | SPI1_CON_MODE_32_MODE_16 | SPI1_CON_CKE | SPI1_CON_CKP |
       SPI1_CON_MSTEN | SPI1_CON_FRMEN | SPI1_CON_FRMPOL | SPI1_CON_FRMSYNC | SPI1_CON_FRMSYPW | SPI1_CON_FRMCNT | SPI1_CON_SPIFE;

    SPI1CON2SET = SPI1_CON2_IGNROV | SPI1_CON2_IGNTUR | SPI1_CON2_AUDEN | SPI1_CON2_AUDMONO | SPI1_CON2_AUDMOD;

    /* Enable SPI1 */
    SPI1CONSET = _SPI1CON_ON_MASK;
}

uint32_t I2S1_LRCLK_Get(void)
{
    // for I2S format, will sync on low to high transition
    volatile uint32_t ret = ((PORTD >> 9) & 0x1);
    return ret;
}
