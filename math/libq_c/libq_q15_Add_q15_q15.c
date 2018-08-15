//*****************************************************************************
// File: libq_q15_Add_q15_q15.c
//
// Description: Add (op1 + op2) or subtract (op1 - op2) two 16-bit 
//   2s-complement fractional operands to produce a 16-bit 2s-complement 
//   fractional result with saturation.
//*****************************************************************************/

#include "libq_c.h"

//*****************************************************************************
// libq_Saturate()
//
// Description:
//   Function used only by libq_c library to  
//   saturate a 32-bit value to 16-bits.
//   --The lower 16-bits of the 32-bit input are limited to within the
//   range of MINFRACT16 <= In <= MAXFRACT16.  
//   --It is assumed that the binary point in the 16-bit output is 
//     aligned with the LOWER 16-bits of the 32 bit input. 
//   --The function is used for overflow control to limit
//   values before they exceed their range.  
//   This function does not produce an overflow status flag.
//
// Arguments:
//   q31 a [in](Q16.15) Input value to be saturated
//
// Return Value:
//   q15 result [return] Saturated value(Q1.15)
//
//*****************************************************************************/
static q15 libq_Saturate(q31 a)
{
    q15 result;   
  
    result = (q15)a;

    if (a > (q31)(MAXFRACT16)) 
    {
        //Overflow--force to MAX positive value
        result = (q15)MAXFRACT16;
    }
    if (a < (q31)(MINFRACT16))
    {
        // overflows in the negative direction-- force to MIN negative value.
        result = (q15)MINFRACT16;
    } 
    return (result);
}


//*****************************************************************************
//
// libq_q15_Add_q15_q15()
//
// Description:
//   Add two 16-bit 2s-complement fractional (op1 + op2) to produce a 16-bit
//   2s-complement fractional result with saturation. 
//   --The saturation is for handling the overflow/underflow cases, 
//     where the result is set to MAX16 when an overflow occurs and the 
//     result is set to MIN16 when an underflow occurs.  
//   --This function does not produce any status flag to indicate when an 
//     overflow or underflow has occured.
//   --It is assumed that the binary point is in exactly the same bit position
//     for both 16-bit inputs and the resulting 16-bit output.  
//
//   This function calls the libq_Sat_q15()
//
// Arguments:
//   q15 a [in] Addition operand op1. (Qn.m)
//   q15 b; [in] Addition operand op2.(Qn.m)
//
// Return Value:
//   q15 result [return]  Range: MINFRACT16 <= result <= MAXFRACT16
//
//*****************************************************************************
q15 libq_q15_Add_q15_q15(q15 a, q15 b)
{
    q31  sumExtP;                   /* Value returned */
  
    //Sign-Extend inputs to 32-bits before 'adding' so that the addition is
    //extended-precision for overflow bits.
    sumExtP = (q31)a + (q31)b;
    //Saturate the 32-bit sum to eliminate any overflow bits
    //--Result is closest 16 bit value
    return (libq_Saturate(sumExtP));
}


/******************************************************************************
 *
 * libq_q15_Sub_q15_q15()
 *
 * Description:
 *   Subtract two 16-bit 2s-complement fractional (op1 - op2) to produce a
 *   16-bit 2s-complement fractional difference result with saturation. 
 *   --The saturation is for handling the overflow/underflow cases, 
 *     where the result is set to MAX16 when an overflow occurs and 
 *     the result is set to MIN16 when an underflow occurs.  
 *   --This function does not produce any status flag to indicate when 
 *     an overflow or underflow has occured.
 *   --It is assumed that the binary point is in exactly the same bit position
 *   for both 16-bit inputs and the resulting 16-bit output.  
 * 
 *   This function relates to the ETSI sub function.
 *
 *   This function calls the libq_Saturate() function.
 *
 * Arguments:
 *   q15 a [in] Subtraction operand op1.
 *   q15 b [in] Subtraction operand op2.
 *
 * Return Value:
 *   q15 result [return]  Range: MINFRACT16 <= result <= MAXFRACT16
 *
 ******************************************************************************/
q15 libq_q15_Sub_q15_q15(q15 a, q15 b)
{
  q31  diffExtP;                   /* Value returned */

    // Sign-Extend inputs to 32-bits before 'subtracting' 
    // --subtraction is done with extended-precision to 
    //   accomodate for any overflow bits. 
    diffExtP = (q31)a - (q31)b;
    // Saturate the 32-bit difference to eliminate any overflow bits 
    // --output will be within the allowable 16-bit range. */
    return (libq_Saturate(diffExtP));
}