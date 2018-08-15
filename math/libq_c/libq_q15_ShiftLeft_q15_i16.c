//*****************************************************************************
// File: libq_q15_ShiftLeft_q15_i16.c
//
// Description: Performs an 'Arithmetic' Left Shift of the 16-bit input
//   argument 'a' by the input argument 'b' bit positions.
//*****************************************************************************

#include "libq_C.h"

/******************************************************************************
 * libq_q15_ShiftLeft_q15_i16()
 *
 * Description:
 *   Performs an 'Arithmetic' Shift of the 16-bit input argument 'a' left
 *   by the input argument 'b' bit positions.  If 'b' is a positive number,
 *   a 16-bit left shift is performed with 'zeros' inserted to the right
 *   of the shifted bits.  If 'b' is a negative number, a right shift by abs(b)
 *   positions with 'sign extention' is done by calling the frsisShr() function.
 *   Saturation is applied if shifting causes an overflow or an underflow.
 *   This function relates to the ETSI shl function.
 *
 * Parameters:
 *   q15 a [in]  16-bit signed integer value to be shifted
 *   i16 b [in]  16-bit signed integer shift index
 *          positive value: # of bits to left shift (zeros inserted at LSB's)
 *                          {To not always saturate:
 *                              if 'a=0', then max b=15, else max b=14}
 *          negative value: # of bits to right shift (sign extend)
 *
 * Return Value:
 *   q15 result [return]  Arithmetically shifted 16-bit signed integer output
 ******************************************************************************/
q15 libq_q15_ShiftLeft_q15_i16(q15 a, i16 b)
{
    q15 result;   
    q31 temp;    
  
    if (b >= 0)
    {
        //Shift Left
        //--Create a temporary 32-bit value which contains 'a' shifted left from
        //  the lower 16-bits into the upper 16-bits by 'b' bit positions.
        //  This is done by a 32-bit multiply on sign extended 'a' times the
        //  'desired shifted LSB bit position'. 
        temp  = (q31)a * (q31)(1 << b);
        //overflow/underflow conditions.  
        //--If 'a' was not zero and the LSB 'bit1' is shifted to or past the 
        //   output sign bit, (i.e. an overflow will occur if bit1 is shifted 
        //   to or past bit16).  
        //--Overflow when the sign of the original input 'a' and the sign of 
        //  the output shifted value are different.  If an Overflow/Underflow 
        //  occurred, then saturate the output value to the Max 16-bit positive 
        //  value if input 'a' was positive, and set the output value to the 
        //  Min 16-bit negative value if 'a' was negative. 
        if ( ((b >= (NUMBITSFRACT16 - 1)) && (a != 0)) || \
            (temp != (q31)((q15)temp)))
        {
            result = (a > 0) ? (q15)MAXFRACT16: (q15)MINFRACT16;
        }
        else
        {
            //No Overflow/Underflow
            result = (q15)temp;
        }
    }
    else
    {
        //Shift Right
        if (b != MININT16)
        {
            result = libq_q15_ShiftRight_q15_i16(a,(int16_t)(-b));
        }
        else
        {
            result = libq_q15_ShiftRight_q15_i16(a,(int16_t)(MAXINT16));
        }
    }
    return (result);
}