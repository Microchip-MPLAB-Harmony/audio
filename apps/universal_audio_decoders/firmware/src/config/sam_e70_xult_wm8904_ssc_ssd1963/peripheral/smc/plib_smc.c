/*******************************************************************
  Company:
    Microchip Technology Inc.
    Memory System Service SMC Initialization File

  File Name:
    plib_smc.c

  Summary:
    Static Memory Controller (SMC).
    This file contains the source code to initialize the SMC controller

  Description:
    SMC configuration interface
    The SMC System Memory interface provides a simple interface to manage 8-bit and 16-bit
    parallel devices.

  *******************************************************************/

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
#include "plib_smc.h"
#include "device.h"


/* Function:
    void SMC_Initialize( void )

  Summary:
    Initializes hardware and data for the given instance of the SMC module.

  Description:
    This function initializes the SMC timings according to the external parralel device requirements.

  Returns:
  None.
 */

void SMC_Initialize( void )
{


    /* Chip Select CS0 Timings */
    /* Setup SMC SETUP register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_SETUP= SMC_SETUP_NWE_SETUP(4) | SMC_SETUP_NCS_WR_SETUP(0) | SMC_SETUP_NRD_SETUP(2) | SMC_SETUP_NCS_RD_SETUP(0);

    /* Setup SMC CYCLE register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_CYCLE= SMC_CYCLE_NWE_CYCLE(3) | SMC_CYCLE_NRD_CYCLE(110);

    /* Setup SMC_PULSE register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_PULSE= SMC_PULSE_NWE_PULSE(4) | SMC_PULSE_NCS_WR_PULSE(10) | SMC_PULSE_NRD_PULSE(63) | SMC_PULSE_NCS_RD_PULSE(63);

    /* Setup SMC MODE register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_MODE= SMC_MODE_EXNW_MODE_DISABLED    \
                                           | SMC_MODE_READ_MODE_Msk  | SMC_MODE_DBW_16_BIT | SMC_MODE_BAT_BYTE_SELECT;





} /* SMC_Initialize */

/*******************************************************************************
 End of File
*/
