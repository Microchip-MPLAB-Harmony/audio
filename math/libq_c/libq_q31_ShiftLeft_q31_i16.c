//*****************************************************************************
// File: libq_q31_ShiftLeft_q31_i16.c
//
// Description: Performs an 'Arithmetic' Left Shift of the 32-bit input
//   argument 'a' by the input argument 'b' bit positions.
//*****************************************************************************/

#include "libq_c.h"

/******************************************************************************
 *
 * libq_q31_ShiftLeft_q31_i16()
 *
 * Description:
 *   Performs an 'Arithmetic' Shift of the 32-bit input argument 'a' left
 *   by the input argument 'b' bit positions.  If 'b' is a positive number,
 *   a 32-bit left shift is performed with 'zeros' inserted to the right
 *   of the shifted bits.  If 'b' is a negative number, a 32-bit right shift
 *   by b bit positions with 'sign extention' is done by calling the '..Shr()'
 *   function.  Saturation is applied if shifting causes an overflow or an
 *   underflow.
 * 
 *   This function relates to the ETSI L_shl function.
 *
 * Arguments:
 *   q31 a [in]  32-bit signed integer value to be shifted
 *   Int16 b [in]  16-bit signed integer shift index
 *          positive value: # of bits to left shift (zeros inserted at LSB's)
 *          negative value: # of bits to right shift (sign extend)
 *
 * Return Value:
 *   q31 result [return]  Arithmetically shifted 32-bit signed integer output
 ******************************************************************************/
q31 libq_q31_ShiftLeft_q31_i16(q31 a, i16 b)
{
    q31 result;
    q31 temp; 
  
    /* Check the input argument 'b', the shift index, for the direction to shift.
     * If 'b' is a positive value then perform the shift to the "LEFT". */
    if (b > 0)
    {
        /* Shift Input value left by 'b' which is the default case. */
        result = (q31)(a << b);
        /* Check for overflow/underflow, it happens when 'a' wasn't zero and the
         * LSB 'bit1' is shifted to or past the output sign bit position,
         * (i.e. an overflow will occur if bit1 is shifted to or past bit32).
         * If an Overflow/Underflow occurred, then saturate the output value to
         * Max 32-bit positive value if input 'a' was positive, and set it equal
         * to the Min 32-bit negative value if 'a' was negative. */
        if ((b >= (NUMBITSFRACT32)) && (a != 0))
        {
            result = (a < 0) ? (q31)MINFRACT32: (q31)MAXFRACT32;
        }
        /* Also overflow will occur 'if all the bits are not all the same' between
         * the original sign bit (bit32) and the bit that will become the sign
         * bit after 'b' left shifts. */
        else
        {
            temp = (((q31)MSBBITFRACT32 >> b) & (q31)a);
            if ((a > 0) && (temp != 0))
            {
              result = (q31)MAXFRACT32;
            }
            if ((a < 0) && (temp != ((q31)MSBBITFRACT32 >> b)))
            {
              result = (q31)MINFRACT32;
            }
        }
    }
    /* Else, the shift index 'b' was negative or zero so perform a long RIGHT
     * shift on 'a' by 'b' bits with sign extention and saturation applied. */
    else
    {
        if (b != MININT16)
        {
          result = libq_q31_ShiftRight_q31_i16(a,(int16_t)(-b));
        }
        else
        {
          result = libq_q31_ShiftRight_q31_i16(a,(int16_t)(MAXINT16));
        }
    }
    return (result);
}