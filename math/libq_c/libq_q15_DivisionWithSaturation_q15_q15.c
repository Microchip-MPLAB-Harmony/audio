//*****************************************************************************
// File: libq_q15_DivisionWithSaturation_q15_q15.c
//
// Description: Performs fractional division with saturation.
//*****************************************************************************/

#include "libq_c.h"

//*****************************************************************************
// libq_Subc()
//
// Description:
//   Private function used only by this libq_c library to perform
//   one iteration (step) of fractional division routine.
//
// Arguments:
//   q31 num; [in] Iteration 1: contains the numerator in the upper 16-bits.
//                 Iteration x: for the subsequent iterations it contains the
//                       partial result in the lower 16-bits and partial
//                       remainder in the upper 16-bits.
//   q15 b; [in] 16-bit fractional denominator
//
// Return Value:
//   q31 iteration result  <partial_remainder><parital_result>
//     [return]  32-bit value same format as num argument. It is composed of
//                       the partial result in the lower 16-bits and partial
//                       remainder in the upper 16-bits.
//*****************************************************************************
static q31 libq_Subc(q31 num, q15 den)
{
    q31 result; 
    q31 temp;  
  
    // Align the numerator and denominator and subtract the two. Shift the
    // denominator so the its first bit aligns to the right on the last
    // numerator bit. 
    temp = num - ((q31)den << (NUMBITSFRACT16-1));
  
    /* If the difference from these is not zero, then have next bit of quotient,
     * 
     * next time. */
    if (temp >= (q31)0)
    {
        //Place the bit into the bottom of the shifted partial result. 
        result = (q31)((temp << 1) + 1);
    }
    else
    {
        //denomintor is too big
        //--so shift the numerator 1 bit 
        result = (q31)(num << 1);
    }
    return (result);
}


/******************************************************************************
 *
 * libq_q15_DivisionWithSaturation_q15_q15()
 *
 * Description:
 *   Performs fractional division with saturation with these restrictions:
 *   1. Both the numerator and denominator must be positive.
 *   2. In order to obtain a non-saturated result, the numerator must be LESS
 *      than or equal to the denominator.
 *   3. The denominator must not equal zero.
 *   4. If 'num' equals 'den', then the result equals MAXINT16.
 * 
 *   This function relates to the ETSI div_s function.
 *
 * Arguments:
 *   q15 num; [in] 16-bit fractional numerator
 *   q15 den; [in] 16-bit fractional denumerator
 *
 * Return Value: Ration a/b in 16-bit fractional format
 ******************************************************************************/
q15 libq_q15_DivisionWithSaturation_q15_q15 (q15 num, q15 den)
{
    q15 result;   
    q31 partRes; 
    int16_t signFlag;
    int16_t i;      
  
  
    //Final sign 
    signFlag = (((q31)num * den) > 0) ? 0: 1;
  
    // Division peformed unsigned.
    // --Convert Numerator value into 32-bits for Subc routine. */
    den  = libq_q15_Abs_q15(den);
    partRes = libq_q31_Abs_q31((q31)num);
  
    if(partRes >= den)
    {
      //Denominator > numerator
      result = (q15)((signFlag != 0) ? MININT16: MAXINT16);
    }
    else
    {
      // Shift the starting numerator up into the top 16 bits. */
      partRes = (partRes << NUMBITSFRACT16);

      //Perform a 'One-Bit' division for each bit in the denominator.
      //The 'subc' routine is a 'conditional subtraction' local function.  It
      //performs a 'one'bit' division on the input partial result and returns
      //the new partial result.  The partRes input value for the 'First' interation
      //holds the numerator in the top 16-bits and zeros in the lower 16-bits.
      //Every iteration after that it holds the current partial remainder in
      //the top 16-bits and the partial result in the lower 16-bits.
      for (i = 0; i < NUMBITSFRACT16; i++)
      {
          partRes = libq_Subc(partRes, den);
      }
      //The 32-bit partial result holds the remainder in the top 16-bits and the
      //quotient in the lower 16-bits.  Take just the quotient bits.  Shift
      //left by one to account for the fact that division was done as unsigned.
      //This puts the sign bit position back in. */
      partRes = partRes & (uint16_t)(BITMASKFRACT16);
      partRes = (partRes >> 1);
      //Based on original signs, if output is suppose to be negative, 
      //take negative of the partial result. 
      if (signFlag)
      {
          partRes = (-partRes);
      }
      /* Truncate the 32-bit parital result to the 16-bit output result */
      result = (q15)partRes;
    }
  return (result);
}