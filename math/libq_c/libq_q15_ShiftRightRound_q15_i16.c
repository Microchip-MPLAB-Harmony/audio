//*****************************************************************************
// File: libq_q15_ShiftRightRound_q15_i16.c
//
// Description: Performs an 'Arithmetic' Right Shift of the 16-bit input 
//   argument 'a' by the input argument 'b' bit positions with Rounding applied.
//*****************************************************************************/

#include "libq_c.h"

/******************************************************************************
 * libq_q15_ShiftRightRound_q15_i16()
 *
 * Description:
 *
 * Description:
 *   Performs an 'Arithmetic' RIGHT Shift on a 16-bit input by 'b' bits with 
 *   Rounding applied.  The rounding occurs by adding a bit weight of "1/2 Lsb",
 *   where the "Lsb" is the Ending (shifted) Lsb. For example: The initial 
 *   Bit#(b) is after the right shift Bit#(0), so the rounding bit weight is
 *   Bit#(b-1).  
 *
 *   Rounding does not occur on either left shifts or on no shift needed cases. 
 *   For positive shift directions (b>0), 'b' Lsb-bits are shifted out 
 *   to the right and 'b' sign-extended Msb-bits fill in from the left.  
 *   For negative shift directions (b<0), 'b' Lsb's are shifted to the LEFT 
 *   with 0's filling in the empty lsb position. The left shifting causes 
 *   'b' Msb-bits to fall off to the left, saturation is applied to any shift 
 *   left value that overflows. This function calls the '..Shl()' function 
 *   to perform the actual 16-bit left shift.  This function does not provide 
 *   any status-type information to indicate when overflows occur.
 *
 *   This function relates to the ETSI shr_r function.
 *
 * Arguments:
 *   q15 a [in]  16-bit signed integer value to be shifted
 *   i16 b [in]  16-bit signed integer shift index 
 *          positive value: # of bits to right shift (sign extend)
 *                          {b > 15, results in all sign bits}  
 *          negative value: # of bits to left shift (zeros inserted at LSB's)
 *
 * Return Value:
 *   q15 result [return]  Arithmetically shifted 16-bit signed integer output 
 ******************************************************************************/
q15 libq_q15_ShiftRightRound_q15_i16(q15 a, i16 b)
{  
    q15 result;  
    q15 rndNum; 
      
    if (b > (NUMBITSFRACT16 - 1))
    {
        //'Msb' shifts past the 'Lsb'
        // --After shifting & ROUNDING the result is always '0'
        result = 0;
    }
    else
    {
        //Standard Right Shift
        result = libq_q15_ShiftRight_q15_i16(a, b);
        if (b > 0)
        {
            //The Round weight is '1/2 of an ENDING Lsb', i.e. Bit'b' shifts 
            //to Bit'0' so the preshifted rounding weigth is Bit'b-1'. Adding in 
            //the rounding weight will only change the results if the original 
            //'1/2 Lsb' Bit'b-1' is set.
            if ((a & ((q15)1 << (b - 1))) != 0)
            {
                //The 1/2 Lsb bit is set to round up by a full '1' Lsb.  Saturate
                //any overflow cases. 
                rndNum = (q15)1;
                result = libq_q15_Add_q15_q15(result, rndNum);  
            }
        }
    }
    return (result);
}