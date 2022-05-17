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
// File: libq_q15_Negate_q15.c
//
// Description: Negate 16-bit 2s-complement fractional value with saturation.
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

/******************************************************************************
 * libq_q15_Negate_q15()
 *
 * Description:
 *   Negate 16-bit 2s-complement fractional value with saturation. The
 *   saturation is for handling the case where negating a MINFRACT16 is
 *   greater than MAXFRACT16, or the allowable range of values.
 *   This function relates to the ETSI negate function.
 *
 * Arguments:
 *   q15 a [in] input argument
 *
 * Return Value:
 *   q15 result [return]  Range: MINFRACT16 <= result <= MAXFRACT16
 ******************************************************************************/
q15 libq_q15_Negate_q15(q15 a)
{
    q15 result;
  
    if (a != (q15)MINFRACT16)
    {
      //Simply take the negative 2's complement value 
      result = (q15)(-a);
    }
    else
    {
      //Else for the max negative value, saturate to the most positive value 
      result = (q15)MAXFRACT16;
    }
    return (result);
}
