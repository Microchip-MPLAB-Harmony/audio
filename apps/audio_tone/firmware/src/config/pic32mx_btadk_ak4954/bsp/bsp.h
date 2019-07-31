/*******************************************************************************
  Board Support Package Header File.

  Company:
    Microchip Technology Inc.

  File Name:
    bsp.h

  Summary:
    Board Support Package Header File 

  Description:
    This file contains constants, macros, type definitions and function
    declarations 
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

#ifndef _BSP_H
#define _BSP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "device.h"

// *****************************************************************************
// *****************************************************************************
// Section: BSP Macros
// *****************************************************************************
// *****************************************************************************
/*** LED Macros for LED5 ***/
#define LED5_Toggle() (LATAINV = (1<<9))
#define LED5_Get() ((PORTA >> 9) & 0x1)
#define LED5_On() (LATACLR = (1<<9))
#define LED5_Off() (LATASET = (1<<9))
/*** LED Macros for LED1 ***/
#define LED1_Toggle() (LATAINV = (1<<4))
#define LED1_Get() ((PORTA >> 4) & 0x1)
#define LED1_On() (LATACLR = (1<<4))
#define LED1_Off() (LATASET = (1<<4))
/*** LED Macros for LED2 ***/
#define LED2_Toggle() (LATAINV = (1<<5))
#define LED2_Get() ((PORTA >> 5) & 0x1)
#define LED2_On() (LATACLR = (1<<5))
#define LED2_Off() (LATASET = (1<<5))
/*** LED Macros for LED3 ***/
#define LED3_Toggle() (LATAINV = (1<<6))
#define LED3_Get() ((PORTA >> 6) & 0x1)
#define LED3_On() (LATACLR = (1<<6))
#define LED3_Off() (LATASET = (1<<6))
/*** LED Macros for LED4 ***/
#define LED4_Toggle() (LATAINV = (1<<7))
#define LED4_Get() ((PORTA >> 7) & 0x1)
#define LED4_On() (LATACLR = (1<<7))
#define LED4_Off() (LATASET = (1<<7))
/*** SWITCH Macros for SWITCH1 ***/
#define SWITCH1_Get() ((PORTA >> 0) & 0x1)
#define SWITCH1_STATE_PRESSED 0
#define SWITCH1_STATE_RELEASED 1
/*** SWITCH Macros for SWITCH3 ***/
#define SWITCH3_Get() ((PORTA >> 10) & 0x1)
#define SWITCH3_STATE_PRESSED 0
#define SWITCH3_STATE_RELEASED 1
/*** SWITCH Macros for SWITCH2 ***/
#define SWITCH2_Get() ((PORTA >> 1) & 0x1)
#define SWITCH2_STATE_PRESSED 0
#define SWITCH2_STATE_RELEASED 1
/*** SWITCH Macros for SWITCH4 ***/
#define SWITCH4_Get() ((PORTB >> 12) & 0x1)
#define SWITCH4_STATE_PRESSED 0
#define SWITCH4_STATE_RELEASED 1
/*** SWITCH Macros for SWITCH5 ***/
#define SWITCH5_Get() ((PORTB >> 13) & 0x1)
#define SWITCH5_STATE_PRESSED 0
#define SWITCH5_STATE_RELEASED 1
/*** SWITCH Macros for SWITCH6 ***/
#define SWITCH6_Get() ((PORTB >> 14) & 0x1)
#define SWITCH6_STATE_PRESSED 0
#define SWITCH6_STATE_RELEASED 1




// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void BSP_Initialize(void)

  Summary:
    Performs the necessary actions to initialize a board

  Description:
    This function initializes the LED and Switch ports on the board.  This
    function must be called by the user before using any APIs present on this
    BSP.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Example:
    <code>
    //Initialize the BSP
    BSP_Initialize();
    </code>

  Remarks:
    None
*/

void BSP_Initialize(void);

#endif // _BSP_H

/*******************************************************************************
 End of File
*/
