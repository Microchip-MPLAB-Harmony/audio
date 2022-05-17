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
// File: libq_q15_MsuR_q31_q15_q15.c
//
// Description: This function is like frdssdMsu() but WITH Rounding applied to 
//   the subtractor result before it is saturated and the top 16-bits taken.
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

/******************************************************************************
 * libq_q15_MsuR_q31_q15_q15()
 *
 * Description:
 *   This function is like frdssdMsu() but WITH Rounding applied to the 
 *   subtractor result before it is saturated and the top 16-bits taken.
 *   This function first multiplies the two 16-bit input values 'b x c' which 
 *   results in a 32-bit value.  This result is left shifted by one to account 
 *   for the extra sign bit inherent in the fractional-type multiply. So, the
 *   shifted number now has a '0' in the Lsb. The shifted multiplier output
 *   is then SUBTRACTED from the 32-bit fractional input 'a'. Then the 32-bits
 *   output from this subtraction are rounded by adding '2^15'.  This value 
 *   is then saturated to be within the q15 range.  
 *   It is assumed that the binary point of the 32-bit input value a is in
 *   the same bit position as the shifted multiplier output.
 *   The MsuR function is for fractional Q-type format data only and
 *   it therefore will not give the correct results for true integers. 
 * 
 *   This function relates to the ETSI msu_r function.
 *
 * Arguments:
 *   q31 a [in]  32-bit 32-bit Value which is subtracted from
 *   q15 b [in]  16-bit multiplication operand 1
 *   q15 c [in]  16-bit multiplication operand 2
 *
 * Return Value:
 *   q15 result [return] 16-bit  
 ******************************************************************************/
q15 libq_q15_MsuR_q31_q15_q15(q31 a, q15 b, q15 c)
{  
  q15 result;  
  q31 msuOut; 

  //Call libq_Msu() which multiplies of the 16-bit inputs, producing a 32-bit 
  //result.  It then shifts the result left by 1 bit (to get rid of the extra 
  //sign bit), and subtracts it from the 32-bit input 'a'.
  msuOut = libq_q31_Msu_q31_q15_q15(a, b, c);

  //Add Fractional Rounding to 32-bit Msu Results, by rounding by 2^15.
  //Then saturate the rounded result.  The libq_Add() function performs both.
  msuOut = libq_q31_Add_q31_q31(msuOut,(q31) ROUNDFRACT32);

  //Extract the upper 16-bits from the rounded 32-bit sum 
  result = libq_q15_ExtractH_q31(msuOut);

  return (result);
}
