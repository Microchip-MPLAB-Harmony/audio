//*****************************************************************************
// File: libq_q31_DepositH_q15.c
//
// Description: Composes a 32-bit fractional value by placing the 16-bit 
//   fractional input value into the upper (frsdDepositH) or lower 
//   (frsdDepositL) 16-bits of the 32-bit composite output value. In addition,
//   frsdDepositH zeroes the lower 16 bits and frsdDepositL sign extends the 
//   upper 16 bits.
//*****************************************************************************

#include "libq_c.h"

/******************************************************************************
 * libq_q31_DepositH_q15()
 *
 * Description:
 *   Composes a 32-bit fractional value by placing the input 16-bit fractional
 *   value in the composite MSB's and zeros the composite 16-bit LSB's
 *   This is a bit-for-bit placement of input 16-bits into the top 32-bit
 *   result.
 * 
 *   This function relates to the ETSI L_deposit_H function.
 *
 * Arguments:
 *   q15 a [in] input argument
 *
 * Return Value:
 *   q31 result [return] Input 16-bits in upper MSB's and zeros in the lower LSB's
 ******************************************************************************/
q31 libq_q31_DepositH_q15(q15 a)
{
    q31 result;   
  
    //Bit-for-Bit placement of the input 16-bits into the upper 16-bits of
    //the 32-bit result.  Zeros are shifted in to fill the lower 16-bits.*/
    result = ((q31)a) << NUMBITSFRACT16;
    return (result);
}


/******************************************************************************
 *
 * libq_q31_DepositL_q15()
 *
 * Description:
 *   Composes a 32-bit fractional value by placing the 16-bit fraction input
 *   value into the lower 16-bits of the 32-bit composite value. The 16-bit
 *   MSB's of the composite output are sign extended. This is a bit-for-bit
 *   placement of input 16-bits into the bottom portion of the composite
 *   32-bit result with sign extention.
 *   This function relates to the ETSI L_deposit_l function.
 *
 * Arguments:
 *   q15 a [in] input argument
 *
 * Return Value:
 *   q31 result [return] SignExtend 16-bit MSB's and Input Value in lower 
 *              16-bit LSB's
 *
 ******************************************************************************/
q31 libq_q31_DepositL_q15(q15 a)
{
    q31 result;
  
    // Bit-for-Bit placement of the input 16-bits into the lower 16-bits of
    // the 32-bit result and sign extending the MSB's.
    result = (q31)a;
    return (result);
}
