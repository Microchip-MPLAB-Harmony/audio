/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

//****************************************************************************
//
// File: libq_q31_Multi_q15_q31.c
//
// Description: 16 bit by 32 bit fractional multiply.
//
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

//*****************************************************************************
//
// libq_q31_Multi_q15_q31()
//
// Description:
//   Implement 16 bit by 32 bit multiply as shown below 
//   The 's' and 'u' notation shows the processing of signed and 
//   unsigned numbers.
//
//        -B1- -B0-   s u     2nd argument is 32 bits
//             -A0-     s     1st argument is 16 bits
//   --------------
//        A0B0 A0B0   s=s*u   1st 32-bit product is A0*B0
//   A0B1 A0B1        s=s*s   2nd 32-bit product is A0*B1
//   --------------
//   -S2- -S1- -S0-   s=s+s   48-bit result is the sum of products
//   -P1- -P0-                32-bit return is the most significant bits of sum
//
//   The algorithm is implemented entirely with the fractional arithmetic
//   library. The unsigned by signed multiply is implemented by shifting bits
//   15:1 to  bits 14:0 of a 16-bit positive fractional number, which throws
//   away bit 0 of the 32-bit number. Since that affects result bits that are
//   used for rounding, rounding processing is included. Saturation processing
//   is handled implicitly in the fractional arithmetic library, except for the
//   case of maximum negative numbers.
//
// Arguments:
//   q15 argAQ1d15
//     [in] 16 bit argument.
//   q31 argBQ1d31
//     [in] 32 bit argument.
//
// Return Value:
//   q31 prodQ1d31
//
//
//*****************************************************************************
q31 libq_q31_Multi_q15_q31(q15 argAQ1d15, q31 argBQ1d31)
{
  q15   upper16;     // upper 16 bit half of 32 bit number
  q15   lower16;     // lower 16 bit half of 32 bit number
  q31   prodQ1d31;   // Product of input arguments 

  // Extract the upper 16 bits of the 32 bit argument 
  upper16     = libq_q15_ExtractH_q31(argBQ1d31);

  // Extract bits [15:1] as a positive number into the lower 16 by
  // subtracting the upper16 from the 32 bit argument and shifting once
  prodQ1d31   = libq_q31_Sub_q31_q31(argBQ1d31,libq_q31_DepositH_q15(upper16));
  lower16     = libq_q15_ExtractL_q31(libq_q31_ShiftRight_q31_i16(prodQ1d31,1));

  // If the lsb of the 32-bit arg was lost in the preceding shift,
  // save the 16 bit argument for rounding the lower product 
  prodQ1d31   = (prodQ1d31 & 1) ? libq_q31_DepositL_q15(argAQ1d15) : 0;

  // Compute the product of lower16 and 16 bit argument, with rounding 
  prodQ1d31   = libq_q31_Mac_q31_q15_q15(prodQ1d31, lower16, argAQ1d15);
  prodQ1d31   = libq_q31_ShiftRight_q31_i16(prodQ1d31, 15);

  // Accumulate the product of upper16 and 16 bit argument 
  prodQ1d31   = libq_q31_Mac_q31_q15_q15(prodQ1d31, upper16, argAQ1d15);

  // Account for potential saturation in the upper product 
  if ((upper16 == MINFRACT16) && (argAQ1d15 == MINFRACT16))
    prodQ1d31 = libq_q31_Add_q31_q31(prodQ1d31, (q15) 1);

  return(prodQ1d31);
}
