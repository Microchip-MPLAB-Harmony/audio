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
//
// File: FxNorm.c
//
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

/******************************************************************************
 *
 * Fx32Norm()
 *
 * Description:
 *   Produces then number of left shifts needed to Normalize the 32-bit
 *   fractional input.  If the input 'a' is a positive number, it will produce
 *   the number of left shifts required to normalized it to the range of a
 *   minimum of [(MAXFRACT32+1)/2] to a maximum of [MAXFRACT32].
 *   If the input 'a' is a negative number, it will produce
 *   the number of left shifts required to normalized it to the range of a
 *   minimum of [MINFRACT32] to a maximum of [MINFRACT32/2].
 *   This function does not actually normalize the input, it just produces
 *   the number of left shifts required.  To actually normalize the value
 *   the frdidShl() function should be used with the value returned from
 *   this function.  
 *   This function relates to the ETSI norm_l function.
 *
 * Arguments:
 *   Q31 a
 *     [in]  32-bit signed integer value to be normalized
 *
 * Return Value:
 *   In16 result
 *     [return] The number of left shifts required to normalize the 32-bit 
 *     input.
 *              Range: 0 => result < 32  (i.e. NUMBITSFRACT32)
 *                If a>0: 0x40000000 > Normalilzed Value <= 0x7fffffff
 *                   i.e.  (MAXFRACT32+1)/2 >  aNorm <= MAXFRACT32
 *                If a<0: 0x80000000 >= Normalilzed Value < 0xC0000000
 *                   i.e.  MINFRACT32>=  aNorm < MINFRACT32/2 
 *
 ******************************************************************************/
int16_t Fx32Norm(q31 a)
{
  int16_t result;                   /* Value returned */
  q31 temp;  /* Temp var for partial value as its being shifted */

  /* Set the default shift if 'a=0' and initialize the starting shift 
   * value for all other cases. */
  result = (int16_t)0;
  /* If the input is not zero then calculate the number of left shifts.  */
  if (a != 0)
  {
    /* Check for special case where the input is the smallest negative 
     * value, i.e. all sign bits set.  If true, set to maximum possible shift
     * value of '31', i.e. a=0xffffffff => normalized => aNorm=0x80000000 */
    if (a == (q31)BITMASKFRACT32) 
    {
      result = (int16_t)(NUMBITSFRACT32 - 1);
    }
    else
    {
      /* If 'a' is negative but doen't have all bits set to '1' then
       * take the 1's complement value before calculating the number of 
       * shifts so that the same shift counting loop can be used for 
       * positive and negative values   Then calculate the number of
       * left shifts by shifting this temp value until the a bit that is 
       * 'set' reaches the bit31, the most significant data bit next
       * to the sign bit.  For negative numbers, this is really the 
       * first bit that is 'clear' reaches the bit31.   */
      temp = a < (q31)0 ? (q31)(~a): (q31)a;
      for (; temp < (q31)NORMPOSFRACT32; )
      {
        temp = (q31)(temp << 1);
        result = (int16_t)(result + 1);
      }
    } 
  }
  return (result);
}


// *****************************************************************************
/* Function Fx16Norm:
  
   Summary:
     Normalize the 16-bit fractional value. 

   Description:
     Produces then number of left shifts needed to Normalize the 16-bit
     fully fractional input.  If the input 'a' is a positive number, it will produce
     the number of left shifts required to normalized it to the range of a
     minimum of [(MAXFRACT16+1)/2] to a maximum of [MAXFRACT16].
     If the input 'a' is a negative number, it will produce
     the number of left shifts required to normalized it to the range of a
     minimum of [MINFRACT16] to a maximum of [MINFRACT16/2].
     This function does not actually normalize the input, it just produces
     the number of left shifts required.  To actually normalize the value
     the left shift function should be used with the value returned From
     this function.

     the 16-bit input on range: 0 => result < 16  (i.e. NUMBITSFRACT16)
                  If a>0: 0x4000 > Normalized Value <= 0x7fff
                     i.e.  (MAXFRACT16+1)/2 >  aNorm <= MAXFRACT16
                  If a<0: 0x8000 >= Normalilzed Value < 0xC000
                     i.e.  MINFRACT16>=  aNorm < MINFRACT16/2

     This function relates to the ETSI norm_s function.
  
   Parameters:
     q15 - a in Q1.15 
  
   Returns:
     i16 result - The number of left shifts required to normalize 
*/
int16_t Fx16Norm(q15 a)      
{
  int16_t result;    /* Value returned */
  q15 temp;          /* Temp var for partial value as its being shifted */

  // Set the default shift if 'a=0' and initialize the starting shift 
  // value for all other cases.
  result = (int16_t)0;

  // If the input is not zero then calculate the number of left shifts.  
  if (a != 0)
  {
    // Check for special case where the input is the smallest negative 
    // value, i.e. all sign bits set.  If true, set to maximum possible shift
    // value of '16', i.e. a=0xffff => normalized => aNorm=0x8000
    if (a == (q15)BITMASKFRACT16) 
    {
      result = (int16_t)(NUMBITSFRACT16 - 1);
    }
    else
    {
      /* If 'a' is negative but doen't have all bits set to '1' then
       * take the 1's complement value before calculating the number of 
       * shifts so that the same shift counting loop can be used for 
       * positive and negative values   Then calculate the number of
       * left shifts by shifting this temp value until the a bit that is 
       * 'set' reaches the bit31, the most significant data bit next
       * to the sign bit.  For negative numbers, this is really the 
       * first bit that is 'clear' reaches the bit15.   */
      temp = a < (q15)0 ? (q15)(~a): (q15)a;
      for (; temp < (q15)NORMPOSFRACT16; )
      {
        temp = (q15)(temp << 1);
        result = (int16_t)(result + 1);
      }
    } 
  }
  return (result);
}
