//*****************************************************************************
// File: libq_q31_Add_q31_q31.c
//
// Description: Add two 32-bit 2s-complement fractional (op1 + op2) to produce 
//   a 32-bit 2s-complement fractional result with saturation.
//*****************************************************************************/

#include "libq_c.h"

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
    q31 minusbQ31;
  
    /* Add Operands to get default sum. Set overflow/underflow
     * values later if need be, otherwise this is correct. */
    minusbQ31 = libq_q31_Negate_q31(b);
    result = a + minusbQ31;
  
    /* Check to see if both 32-bit operands have the same sign.
     * Exclusive OR the operands then AND with the 32-bit sign bit flag
     * so that the answer will be zero if they were the same sign. */
    if (((a ^ b) & MSBBITFRACT32) == 0)
    {
      /* If operand a & b have same sign bit, check the sign of
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