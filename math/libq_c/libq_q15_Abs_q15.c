//*****************************************************************************
// File: libq_q15_Abs_q15.c
//
// Description: Creates a saturated Absolute value.
//*****************************************************************************/

#include "libq_c.h"

/******************************************************************************
 *
 * libq_q15_Abs_q15()
 *
 * Description:
 *   Saturated Absolute value.  It takes the absolute value of the
 *   16-bit 2s-complement fractional input with saturation. The saturation is
 *   for handling the case where taking the absolute value of MINFRACT16 is
 *   greater than MAXFRACT16, or the allowable range of 16-bit values.
 *   This function relates to the ETSI abs function.
 *
 * Arguments:
 *   q15 a [in] input argument
 *
 * Return Value:
 *   q15 result [return]  abs(input) <= MAXFRACT16
 ******************************************************************************/
q15 libq_q15_Abs_q15(q15 a)
{
    q15 result;                   /* Value returned */
  
    if (a >= (q15)0)
    {
        result = (q15)(a);
    }
    else
    {
        //Negative value, 
        if (a != (q15)MINFRACT16)
        {
            //Not min value (-1) 
            result = (q15)(-a);
        }
        // The min value (-1), so saturate to max value (1) */
        else
        {
            result = (q15)MAXFRACT16;
        }
    }
    return (result);
}
