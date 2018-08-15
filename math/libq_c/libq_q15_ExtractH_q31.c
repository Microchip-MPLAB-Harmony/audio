//*****************************************************************************
// File: libq_q15_ExtractH_q31.c
//*****************************************************************************/

#include "libq_c.h"

#define BITSFRACT16 (16)

/******************************************************************************
 * libq_q15_ExtractH_q31()
 *
 * Description:
 *   Extracts upper 16 bits of input 32-bit fractional value and returns them
 *   as 16-bit fractional value.  This is a bit-for-bit extraction of the top
 *   16-bits of the 32-bit input.
 *   This function relates to the ETSI extract_h function.
 *
 * Arguments:
 *   q31 a [in] input argument
 *
 * Return Value:
 *   q15 result [return] Upper 16 bits of 32-bit argument a
 ******************************************************************************/
q15 libq_q15_ExtractH_q31(q31 a)
{
    q15 result;             
  
    // Bit-for-Bit extract just the top 16-bits 
    result = (q15)(a >> BITSFRACT16);
    return (result);
}


/******************************************************************************
 * libq_q15_ExtractL_q31()
 *
 * Description:
 *   Extracts lower 16-bits of input 32-bit fractional value and returns them
 *   as 16-bit fractional value.  This is a bit-for-bit extraction of the
 *   bottom 16-bits of the 32-bit input.
 *   This function relates to the ETSI extract_l function.
 *
 * Arguments:
 *   q31 a [in] input argument
 *
 * Return Value:
 *   q15 result [return] Lower 16 bits of 32-bit argument a
 ******************************************************************************/
q15 libq_q15_ExtractL_q31(q31 a)
{
    q15 result;
  
    //Bit-for-Bit extract just the bottom 16-bits 
    result = (q15)a;
    return (result);
}