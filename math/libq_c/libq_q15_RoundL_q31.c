//*****************************************************************************
// File: libq_q15_RoundL_q31.c
//
// Description: Rounds the lower 16-bits of the 32-bit fractional input into a 
//   16-bit fractional value with saturation.
//*****************************************************************************/

#include "libq_c.h"

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