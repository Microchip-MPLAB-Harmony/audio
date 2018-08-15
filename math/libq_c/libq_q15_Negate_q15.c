//*****************************************************************************
// File: libq_q15_Negate_q15.c
//
// Description: Negate 16-bit 2s-complement fractional value with saturation.
//*****************************************************************************/

#include "libq_c.h"

/******************************************************************************
 * libq_q15_Negate_q15()
 *
 * Description:
 *   Negate 16-bit 2s-complement fractional value with saturation. The
 *   saturation is for handling the case where negating a MINFRACT16 is
 *   greater than MAXFRACT16, or the allowable range of values.
 *   This function relates to the ETSI negate function.
 *
 * Arguments:
 *   q15 a [in] input argument
 *
 * Return Value:
 *   q15 result [return]  Range: MINFRACT16 <= result <= MAXFRACT16
 ******************************************************************************/
q15 libq_q15_Negate_q15(q15 a)
{
    q15 result;
  
    if (a != (q15)MINFRACT16)
    {
      //Simply take the negative 2's complement value 
      result = (q15)(-a);
    }
    else
    {
      //Else for the max negative value, saturate to the most positive value 
      result = (q15)MAXFRACT16;
    }
    return (result);
}