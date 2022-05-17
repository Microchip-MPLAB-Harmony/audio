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
// File: libq_q31_Add_q31_q31.c
//
// Description: Add two 32-bit 2s-complement fractional (op1 + op2) to produce 
//   a 32-bit 2s-complement fractional result with saturation.
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

/******************************************************************************
 * libq_q31_Add_q31_q31()
 *
 * Description:
 *   Add two 32-bit 2s-complement fractional (op1 + op2) to produce a 32-bit
 *   2s-complement fractional result with saturation. The saturation is for
 *   handling the overflow/underflow cases, where the result is set to
 *   MAX32 when an overflow occurs and the result is set to
 *   MIN32 when an underflow occurs.  This function does not produce
 *   any status flag to indicate when an overflow or underflow has occured.
 *   It is assumed that the binary point is in exactly the same bit position
 *   for both 32-bit inputs and the resulting 32-bit output.  
 *   This function relates to the ETSI L_add function.
 *
 * Arguments:
 *   q31 a [in] Addition operand op1.
 *   q31 b; [in] Addition operand op2.
 *
 * Return Value:
 *   q31 result [return]  Range: MINFRACT32 <= result <= MAXFRACT32
 *
 ******************************************************************************/
q31 libq_q31_Add_q31_q31(q31 a, q31 b)
{
    q31 result;   
  
    /* Add Operands to get default sum. Set overflow/underflow
     * values later if need be, otherwise this is correct. */
    result = a + b;
    /* Check to see if both 32-bit operands have the same sign.
     * Exclusive OR the operands then AND with the 32-bit sign bit flag
     * so that the answer will be zero if they were the same sign. */
    if (((a ^ b) & MSBBITFRACT32) == 0)
    {
        /* If operand a & b have the same sign bit, check the sign of
         * the result. If the sign of the result is different from both
         * the operands than an overflow or underflow has occurred. */
        if (((result ^ a) & MSBBITFRACT32) != 0)
        {
            /* If the operands where both negative, then an underflow has
             * occurred, so set the result to the most Negative number,
             * MINFRACT32.  If the operands where both positive, than
             * an overflow has occurred, so set the result to the most
             * Positive number, MAXFRACT32. */
            result = (a < (q31)0) ? (q31)MINFRACT32 : \
              (q31)MAXFRACT32;
        }
    }
    return (result);
}


/******************************************************************************
 * libq_q31_Sub_q31_q31()
 *
 * Description:
 *   Subtract two 32-bit 2s-complement fractional (op1 - op2) to produce a 32-bit
 *   2s-complement fractional result with saturation. The saturation is for
 *   handling the overflow/underflow cases, where the result is set to
 *   MAX32 when an overflow occurs and the result is set to
 *   MIN32 when an underflow occurs.  This function does not produce
 *   any status flag to indicate when an overflow or underflow has occured.
 *   It is assumed that the binary point is in exactly the same bit position
 *   for both 32-bit inputs and the resulting 32-bit output.  
 *   This function relates to the ETSI L_add function.
 *
 * Arguments:
 *   q31 a [in] Addition operand op1.
 *   q31 b [in] Addition operand op2.
 *
 * Return Value:
 *   q31 result [return]  Range: MINFRACT32 <= result <= MAXFRACT32
 ******************************************************************************/
q31 libq_q31_Sub_q31_q31(q31 a, q31 b)
{
    q31 result;   
  
    //Add the negative of b to a. Negating b=MAXINT --> overflow of Q31.
    result = (q31)((int64_t)a - (int64_t)b);
  
    // Check to see if both operands have the different sign.
    // -->Exclusive OR the operands then AND with the 32-bit sign bit flag
    // so that the answer will be zero if they were the same sign.
    if (((a ^ b) & MSBBITFRACT32) != 0)
    {
      // If operand a and b have different sign bit, 
      // The result should have the same sign as a.
      // -->If the sign of the result is different from 
      //    a, then overflow or underflow has occurred and the result
      //    is the saturated value for the sign of a
      if (((result ^ a) & MSBBITFRACT32) != 0)
      {
          result = (a < (q31)0) ? (q31)MINFRACT32 : (q31)MAXFRACT32;
      }
    }
    return (result);
}
