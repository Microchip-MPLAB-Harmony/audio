//*****************************************************************************
// File: libq_q31_Msu_q31_q15_q15.c
//
// Description: Performs a Multiply-Subtraction function WITH saturation.
//*****************************************************************************/

#include "libq_C.h"

/******************************************************************************
 * libq_q31_Msu_q31_q15_q15()
 *
 * Description:
 *   Performs a Multiply-Subtraction function WITH saturation.  This routine
 *   returns the fractional 32-bit result from the subtractor output 
 *   'SAT(addOut_Q1d31)=outQ1d15' where 'a_Q1d31 - multOut_Q1d31 = 
 *   addOut_Q1d31', and 'b_Q1d15 x c_Q1d15 = multOut_Q1d31'. The multiply is
 *   performed on the two 16-bit fractional input values 'b x c' which results
 *   in a 32-bit value.  
 *
 *   This result is left shifted by one to account for the extra sign bit
 *   inherent in the fractional-type multiply.  The shifted number represents a
 *   Q1d31 number with the lsb set to '0'.  This Q1d31 number then is
 *   subtracted from the 32-bit fractional input argument 'a'.  Saturation is
 *   applied on the output of the accumulator(Subtractor) to keep the value
 *   within the 32-bit fractional range and then this value is returned.  This
 *   instruction is equalivalent to performing the following functions:
 *         Msu(a,b,c) ====> Sub(a, Mult(b,c)));
 *
 *   The Msu() function is for fractional Qtype format data only and it 
 *   therefore will not give the correct results for true integers. 
 *
 *   This function relates to the ETSI L_msu function.
 *
 * Arguments:
 *   q31 a [in]  32-bit operand that is subtracted from 
 *   q15 b [in]  16-bit multiplication operand 1
 *   q15 c [in]  16-bit multiplication operand 2
 *
 * Return Value:
 *   q31 result [return] 32-bit  
 ******************************************************************************/
q31 libq_q31_Msu_q31_q15_q15(q31 a,q15 b, q15 c) 
{  
    q31 result;  
    q31 multOut;
   
    /* 'b_Q1d15 x c_Q1d15 => multOut_Q1d31'.
     * --The 1 bit shift to the left to get rid of the extra sign bit is
     *   automatically taken care of in the 'frssdMult' function.  */
    multOut = libq_q31_Mult2_q15_q15(b,c);
    /* Subtract the multiplier output multOut_Q1d31 from the 2's complement
     * 32-bit fractional Input, 'a_Q1d31'.  
     * --It is assumed that the binary point
     *   of the 32-bit 'a' input is in the same position as the multOut_Q1d31
     *   value.  */
    result = libq_q31_Sub_q31_q31(a, multOut);

    return (result);
}