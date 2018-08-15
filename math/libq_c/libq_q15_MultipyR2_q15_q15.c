//*****************************************************************************
//
// File: libq_q15_MultipyR2_q15_q15.c
//
// Description: Performs fractional multiplication of two 16-bit fractional 
//   values and returns a rounded 16-bit fractional result.
//
//*****************************************************************************

#include "libq_C.h"


/******************************************************************************
 *
 * libq_q15_MultipyR2_q15_q15()
 *
 * Description:
 *   Performs fractional multiplication of two 16-bit fractional values
 *   and returns a ROUNDED 16-bit fractional result. The function performs
 *   a Q15xQ15->Q30 bit multiply with a left shift by '1' to give a Q31 
 *   result.  This automatic shift left is done to get rid of the extra sign 
 *   bit that occurs in the interpretation of the fractional multiply result.
 *   Saturation is applied to any 32-bit result that overflows. Rounding
 *   is applied to the 32-bit SHIFTED result by adding in a weight factor
 *   of 2^15, again any overflows are saturated.  The TOP 16-bits are
 *   extracted and returned.  This function is for fractional 'Qtype' data 
 *   only and it therefore will not give the correct results for 
 *   true integers (because left shift by '1').  This function assumes that
 *   the binary point in the 32-bit shifted multiplier ouput is between
 *   bit_16 and bit_15 when the rounding factor is added. For the special 
 *   case where both inputs equal the MINFACT16, the function returns a 
 *   value equal to MAXFACT16, i.e. 0x7fff = libq_q15_Mult_q15_q15(0x8000,0x8000). 
 *   This function internally calls the libq_q15_Mult_q15_q31() routine to perform
 *   the actual multiplication and the libq_q15_Round_q15_q15() routine to perform the
 *   actual rounding. 
 * 
 *   This function relates to the ETSI mult_r function.
 *
 * Arguments:
 *   q15 a
 *     [in]  16-bit signed integer (fract_Q1d15) operand 1
 *   q15 b
 *     [in]  16-bit signed integer (fract_Q1d15) operand 2
 *
 * Return Value:
 *   q15 result
 *     [return]  rounded 16-bit signed integer (fract_Q1d15) output value 
 *
 ******************************************************************************/
q15 libq_q15_MultipyR2_q15_q15(q15 a, q15 b)
{
  /* Mutliply Q1d15 x Q1d15->Q1d30 and shift left by one for Q1d31. 
   * Round the Q1d31 result by adding in 1/2 of Q1d15 lsb to the Q1d31 value.
   * Extract and return the TOP 16-bits */
  return (libq_q15_RoundL_q31(libq_q31_Mult2_q15_q15(a,b)));
}
