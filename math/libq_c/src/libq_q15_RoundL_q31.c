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
// File: libq_q15_RoundL_q31.c
//
// Description: Rounds the lower 16-bits of the 32-bit fractional input into a 
//   16-bit fractional value with saturation.
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

/******************************************************************************
 * libq_q15_RoundL_q31()
 *
 * Description:
 *   Rounds the lower 16-bits of the 32-bit fractional input into a 16-bit
 *   fractional value with saturation. This converts the 32-bit fractional
 *   value to 16-bit fractional value with rounding.  This function calls the
 *   '..Add..' function to perform the 32-bit rounding of the input value and
 *   'ExtractH' function to extract to top 16-bits.  This has the effect of
 *   rounding positive fractional values up and more positive, and has the 
 *   effect of rounding negative fractional values up and more positive.
 *   This function relates to the ETSI round function.
 *
 * Arguments:
 *   q31 a [in] input 32-bit fractional argument
 *
 * Return Value:
 *   q15 result [return]  rounded 16-bit fractional value
 ******************************************************************************/
q15 libq_q15_RoundL_q31(q31 a)
{
  q15 result; 
  q31 temp;  

  //Add Fractional Rounding bit to 32-bit input so that the upper 16-bits
  //of the input round up.  Saturation is part of the addition. 
  temp = libq_q31_Add_q31_q31(a,(q31)ROUNDFRACT32);

  // Extract the upper 16-bits from the rounded 32-bit sum.
  result = libq_q15_ExtractH_q31(temp);

  return (result);
}
