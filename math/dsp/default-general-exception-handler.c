/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

/*********************************************************************
 *
 *        Default General Exception Handler Implementation
 *
 *********************************************************************
 * Filename:        default-general-exception-handler.c
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB C Compiler for PIC32 MCUs
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 ********************************************************************/
/* This is a default definition of a general exception handler function.
 * It does nothing and just goes into an infinite loop. If the user
 * application supplies a handler function, this function will not be
 * referenced and thus not pulled in from the library.
 */

#include <stdint.h>

typedef struct _XCPT_FRAME  // access to all major registers from the instruction that caused the exception
{
    uint32_t at;
    uint32_t v0;
    uint32_t v1;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t t7;
    uint32_t t8;
    uint32_t t9;
    uint32_t ra;
    uint32_t lo;
    uint32_t hi;
    uint32_t cause;
    uint32_t status;
    uint32_t epc;

}XCPT_FRAME;

static enum {
    EXCEP_IRQ = 0,	// interrupt, 0x0
    EXCEP_AdEL = 4,	// address error exception (load or ifetch), 0x4
    EXCEP_AdES,         // address error exception (store), 0x5
    EXCEP_IBE,          // bus error (ifetch), 0x6
    EXCEP_DBE,          // bus error (load/store), 0x7
    EXCEP_Sys,          // syscall, 0x8
    EXCEP_Bp,           // breakpoint, 0x9
    EXCEP_RI,           // reserved instruction, 0xA
    EXCEP_CpU,          // coprocessor unusable, 0xB
    EXCEP_Overflow,	// arithmetic overflow, 0xC
    EXCEP_Trap,         // trap (possible divide by zero), 0xD
    EXCEP_IS1 = 16,	// implementation specfic 1
    EXCEP_CEU,          // CorExtend Unuseable
    EXCEP_C2E           // coprocessor 2
} _excep_code;

static unsigned int _excep_code;
static unsigned int _excep_addr;

void 
__attribute__((weak, nomips16)) _general_exception_handler (XCPT_FRAME* const pXFrame)
{
   
    _excep_code = pXFrame->cause;   // capture exception type

    _excep_code = (_excep_code & 0x0000007C) >> 2;

    if (_excep_code == EXCEP_Overflow)
    {

    pXFrame->v0 = 0x7FFF;           // set function output to maximum (saturation)
    pXFrame->v1 = 0x7FFF;           // set intermediate results to maximum (saturation)
    pXFrame->epc = pXFrame->epc + 4; // set return from exception to next instructino (skip)

    }

    return;

    while (1) {
        // we should only end up here if our exception generates an exception

    }
}
