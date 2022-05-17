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

//*****************************************************************************
// File: libq_q15_ExtractH_q31.c
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

#define BITSFRACT16 (16)

/******************************************************************************
 * libq_q15_ExtractH_q31()
 *
 * Description:
 *   Extracts upper 16 bits of input 32-bit fractional value and returns them
 *   as 16-bit fractional value.  This is a bit-for-bit extraction of the top
 *   16-bits of the 32-bit input.
 *   This function relates to the ETSI extract_h function.
 *
 * Arguments:
 *   q31 a [in] input argument
 *
 * Return Value:
 *   q15 result [return] Upper 16 bits of 32-bit argument a
 ******************************************************************************/
q15 libq_q15_ExtractH_q31(q31 a)
{
    q15 result;             
  
    // Bit-for-Bit extract just the top 16-bits 
    result = (q15)(a >> BITSFRACT16);
    return (result);
}


/******************************************************************************
 * libq_q15_ExtractL_q31()
 *
 * Description:
 *   Extracts lower 16-bits of input 32-bit fractional value and returns them
 *   as 16-bit fractional value.  This is a bit-for-bit extraction of the
 *   bottom 16-bits of the 32-bit input.
 *   This function relates to the ETSI extract_l function.
 *
 * Arguments:
 *   q31 a [in] input argument
 *
 * Return Value:
 *   q15 result [return] Lower 16 bits of 32-bit argument a
 ******************************************************************************/
q15 libq_q15_ExtractL_q31(q31 a)
{
    q15 result;
  
    //Bit-for-Bit extract just the bottom 16-bits 
    result = (q15)a;
    return (result);
}
