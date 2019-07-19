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

#include "plib_${I2S_INSTANCE_NAME?lower_case}.h"

// *****************************************************************************
// *****************************************************************************
// Section: ${I2S_INSTANCE_NAME} Implementation
// *****************************************************************************
// *****************************************************************************

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