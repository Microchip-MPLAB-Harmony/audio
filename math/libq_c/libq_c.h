/*******************************************************************************
  PIC32 Fixed Point in C (LibQ_C) Library

  Company:
    Microchip Technology Inc.
	
  File Name:
    libq_c.h
	
  Summary:
    C-code fixed point typedefs and math functions. 

  Description:
    Signed fixed point types are defined as follows:

	The libq_c Fixed-Point Math Library provides fixed-point math functions 
    written in C for portability between core processors.

    Signed fixed point types (fractional Q types specified by Qn.m) are named
    as follows in the library names:

    Qndm where:
      * n is the number of data bits to the left of the radix point
      * m is the number of data bits to the right of the radix point
      * a signed bit is implied (unless stated otherwise)
      *
    Unique variable types for fractional representation are also defined:

        Exact Name     # Bits Required    Type
         Q0.15  (Q15)          16         fract_q0d15_t
new      Q3.12  (Q15)          16         fract_q3d12_t (radian angles)
new      Q9.6   (Q15)          16         fract_q9d6_t  (degree angles)
new      Q10.5  (Q15)          16         fract_q10d6_t (pixel coordinates, -1024 <= x < 1023)
         Q0.31  (Q31)          32         fract_q0d31_t
         Q15.16 (Q31)          32         fract_q15d16_t
         Q16.15 (Q31)          32         fract_q16d15_t
         Q0.63  (Q64)          64         fract_q0d63_t

    The following block floating point types are supported:
      Mantissa/Exponent
         15     16             32         fract_q15bfp_t
         31     32             64         fract_q31bfp_t


    For convenience, short names are also defined for arbitrary scaled
    fractional types:

    q15 is signed fractional 16 bit value 
    q31 is signed fractional 32 bit value
    i16 is signed integer, i.e. Q16d0

    In addition,  A pseudo floating point 32 bit format (FxQFloat32) 
    is defined that consists of 16 mantissa and a 16 bit exponant (base 2).

    The following block floating point types are supported:
      Mantissa/Exponent
         15     16             32         fract_q15bfp_t
         31     32             64         fract_q31bfp_t


    Functions in the library are prefixed with the type of the return
    value and followed by argument types (in order): 

       libq_<Return Type>_<Function Name>_<Argument Type(s)>: libq_q15_sin_Q2d13

    For example, libq_q1d15_Sin_q10d6 returns a Q1.15 value equal to the
    to the sine of an angle specified as a Q10.6 value (in degrees between
    0 and 360)
        
    Argument types do not always match the return type. Refer to the
    function prototype for a specification of its arguments.

    In some cases, both the return type and the argument type are specified
    within the function name. For example,

    For arbitrary scaled types (q15, q16, q31, and q32) the scaling of the
    result will depend on the function and the scaling of the arguments.
    For instance, libq_q15_Add_q15_q15(a,b) will return a scaled value type
    that is the two input types (which must have equivalent scaled value type).


    NOTE: The libq_c functions do not correspond to the libq fixed-point 
          library optimized for the microaptive core processor and written 
          in asm.

    Table of LIBQ_C math functions:

      Sine:
        libq_q1d15_Sin_q10d6
        libq_q20d12_Sin_q20d12
  
      Abs:
        libq_q15_Abs_q15
        libq_q31_Abs_q31
  
      Negate:
        libq_q15_Negate_q15
        libq_q31_Negate_q31
      
      Round:
        libq_q15_RoundL_q31
  
      Deposit:
        libq_q31_DepositH_q15
        libq_q31_DepositL_q15
  
      Extract:
        libq_q15_ExtractH_q31
        libq_q15_ExtractL_q31
  
      Add:
        libq_q15_Add_q15_q15
        libq_q31_Add_q31_q31
  
      Subtract:
        libq_q15_Sub_q15_q15
        libq_q31_Sub_q31_q31
  
      Shift(Scale):
        libq_q15_ShiftLeft_q15_q15
        libq_q31_ShiftLeft_q31_q15
        libq_q15_ShiftRight_q15_q15
        libq_q31_ShiftRight_q31_q15
        libq_q15_ShiftRightR_q15_q15
        libq_q31_ShiftRightR_q31_q15
  
      Multiply:
        libq_q31_Mult2_q15_q15
        libq_q15_MultipyR2_q15_q15 (DEPRECATED - mis-spelled)
        libq_q15_MultiplyR2_q15_q15
        libq_q31_Multi_q15_q31
  
      Divide:
        libq_q15_DivisionWithSaturation_q15_q15
  
      Multiply-Accumulate:
        libq_q31_Mac_q31_q15_q15
        libq_q15_MacR_q31_q15_q15
  
      Multiply-Subtract:
        libq_q31_Msu_q31_q15_q15
        libq_q15_MsuR_q31_q15_q15
  
      Exponential-Averaging:
        libq_q15_ExpAvg_q15_q15_q1d15
         

    Table of LIBQ_C conversion functions:

    Normalize Q value:
      Fx16Norm
      Fx32Norm

    Float-to-Q value:
      Fl2Fract16
      Fl2Fract32
      Fl2FxPnt16
      Fl2FxPnt32
      Fl2FxPnt

    Float-To-Integer:
      Fl2Int16
      Fl2Int32

    Float-To-FxQFloat32:
      Fl2QFloat32
    
    Exponent-To-Float:
      Exponent16ToFloat32
	
  *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END
//

#ifndef _LIBQ_C_H_
#define _LIBQ_C_H_

#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include "float.h"

#include "audio/math/libq_c/include/fract_fixedpoint.h" 

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Fixed point constants

  Summary:
    Fixed point constants for the various fixed point types defined in LibQ.

  Description:
    Maximum and minimum values are defined for each fixed point type.
*/

#define MSBBITFRACT32   MININT32                /* 32-bit Sign Bit */
#define MSBBITFRACT16   MININT16                /* 16-bit Sign Bit */

#define BITMASKFRACT32  (~((uint32_t)0x0L))     /* Bit Mask for 32 */
#define BITMASKFRACT16  (~((uint16_t)0x0))      /* Bit Mask for 16 */

#define NUMBITSFRACT32  ((int16_t)0x020)        /* Num of bits 32  */
#define NUMBITSFRACT16  ((int16_t)0x010)        /* Num of bits 16  */

#define NORMPOSFRACT32  ((q31)0x40000000L)      /* Min +val for 32 */
#define NORMPOSFRACT16  ((q15)0x4000)           /* Min +val for 16 */

#define NORMNEGFRACT32  ((q31)0xc0000000L)      /* Max -val for 32 */
#define NORMNEGFRACT16  ((q15)0xc000)           /* Max -val for 16 */

#define ROUNDFRACT32    ((q31)0x00008000L)      /* Rounding value  */

// Maximum and minimum values for 16-bit data types. 
#define MAXINT16   ((int16_t) 0x7fff)
#define MININT16   ((int16_t) (-MAXINT16 - 1))
#define MAXFRACT16 MAXINT16       /* +0.999969 */
#define MINFRACT16 MININT16       /* -1.000000 */

#define MAXINT32   ((int32_t)0x7fffffffL)
#define MININT32   ((int32_t)(-MAXINT32 - 1))
#define MAXFRACT32 MAXINT32       /* +0.9999999995 */
#define MINFRACT32 MININT32       /* -1.0000000000 */

// minimum and maximum definitions for FX floating point data types 
#define MAXPFLOAT32 {MAXFRACT16, MAXINT16}
#define MINPFLOAT32 {MINFRACT16, MAXINT16}

#define UNITYFLOAT 1.0

/* log10(2) scaled to Q5.11 format */
#define LOG102Q5D11      Fl2FxPnt16(0.301029996,11)
#define NINETYQ10D6      Fl2FxPnt16(90,6)   /* Ninety degrees scaled to Q10d6 */
#define ONEEIGHTYQ10D6   Fl2FxPnt16(180,6)  /* 180 degrees scaled to Q10d6    */
#define TWOSEVENTYQ10D6  Fl2FxPnt16(270,6)  /* 270 degrees scaled to Q10d6    */
#define THREESIXTYQ10D6  Fl2FxPnt16(360,6)  /* 360 degrees scaled to Q10d6    */
#define NINETYQ10D22     Fl2FxPnt32(90,22)  /* Ninety degrees scaled to Q10d22*/
#define ONEEIGHTYQ10D22  Fl2FxPnt32(180,22) /* 180 degrees scaled to Q10d22   */
#define TWOSEVENTYQ10D22 Fl2FxPnt32(270,22) /* 270 degrees scaled to Q10d22   */
#define THREESIXTYQ10D22 Fl2FxPnt32(360,22) /* 360 degrees scaled to Q10d22   */

#define INV_LOG2_10_Q1DOT31 UINT32_C(0x268826a1) // Inverse log base 2 of 10
#define LOG10_2 0x00004D10L // = log10(2), To convert dB into dBFS add this to output of Fract_log10fix

/* find the maximum of two numbers */
#define FrMax(a,b) ((a)>(b)?(a):(b))

/* find the minimum of two numbers */
#define FrMin(a,b) ((a)<(b)?(a):(b))

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Conversion Routines 
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Fixed Point Conversion Routines
 
  Summary:
    Conversions to/from Fixed Point, Floating Point and pseudo Floating Point

  Description:
    Conversions to/from Fixed Point, Floating Point and pseudo Floating Point
*/

// *****************************************************************************
/* Function Fl2Fract16:
  
   Summary:
     Converts floating point constant value to fractional 16-bit value 

   Description
     Converts floating point constant value to fractional 16-bit value with
     rounding.
  
   Parameters:
     value    - Floating point value constant expression to convert.
  
   Returns
     q15 - Equivalent fractional value
  
*/
#define Fl2Fract16(value) \
( \
  (q15)Fl2FxPnt((value), 15) \
)


// *****************************************************************************
/* Function Fl2Fract32:
 

   Summary:
     Converts floating point constant value to fractional 32-bit value 
  
   Description:
     Converts floating point constant value to fractional 32-bit value with
     rounding.
  
   Parameters:
     value - Floating point value constant expression to convert.
  
   Returns
     Equivalent fractional value as AtiFract32.
  
*/
#define Fl2Fract32(value) \
( \
  (q31)Fl2FxPnt((value), 31) \
)


// *****************************************************************************
/* Function Fl2FxPnt16:
  
   Summary:
     Converts floating point constant value to fixed point 16-bit value 

   Description:
     Converts floating point constant value to fixed point 16-bit value with
     rounding.
  
   Parameters:
     value - Floating point value constant expression to convert.
     int_t bits  -- Number of fractional bits.
  
   Returns
     q15 - Equivalent fixed point value
  
*/
#define Fl2FxPnt16(value, bits) \
( \
  (q15)Fl2FxPnt((value), (bits)) \
)

// *****************************************************************************
/* Function Fl2FxPnt32:
  
   Summary:
     Converts floating point constant value to fixed point 32-bit value 

   Description:
     Converts floating point constant value to fixed point 32-bit value with
     rounding.
  
   Parameters:
     value - Floating point value constant expression to convert.
     int_t bits -  Number of fractional bits.
  
   Returns
     q31 - Equivalent fixed point value 
  
*/
#define Fl2FxPnt32(value, bits) \
( \
  (q31)Fl2FxPnt((value), (bits)) \
)


// *****************************************************************************
/* Function Fl2FxPnt:
 
   
   Summary:
     Converts floating point constant value to fixed/fractional value 

   Description:
     Converts floating point constant value to fixed/fractional value of
     specified precision with rounding.
  
   Parameters:
     value       - Floating point value constant expression to convert.
     int_t bits  - Number of fractional bits.
  
   Returns
     double - Equivalent floating point value 
  
*/
#define Fl2FxPnt(value, bits) \
( \
  ((double)(value)*(double)(1UL<<(bits))) + \
    (((double)((double)(value) >= 0.0)) - ((double)0.5)) \
)


// *****************************************************************************
/* Function Fl2Int16:
 
   Summary:
     Converts floating point constant expression to 16-bit integer value
  
   Description:
     Converts floating point constant expression to 16-bit integer value
     Conversion is safe for compilers which assign fractional data type to
     constants with decimal point
  
   Parameters:
     value -   Floating point value constant expression to convert.
  
   Returns
     int_t - Calculated value.
  
*/
#define Fl2Int16(value) ((int16_t)((double)(value)))


// *****************************************************************************
/* Function Fl2Int32:
  
   Summary:
     Converts floating point constant expression to 32-bit integer value

   Description:
     Converts floating point constant expression to 32-bit integer value
     Conversion is safe for compilers which assign fractional data type to
     constants with decimal point
  
   Parameters:
     value - Floating point value constant expression to convert.
  
   Returns
     int_t - Calculated integer value.
  
*/
#define Fl2Int32(value) ((int32_t)((double)(value)))


// *****************************************************************************
/* Function Fl2QFloat32:
  
   Summary:
     Converts a decimal floating-point constant expression to pseudo float

   Description:
     Converts a decimal floating-point constant expression to pseudo float
  
   Parameters:
     mantissa - Floating-point value constant expression to convert to 
                the mantissa portion of the floating-point number.
     exponent - Integer value constant expression to convert to the 
                exponent portion of the floating-point number.
  
   Returns:
     Calculated integer value.
  
*/
#define Fl2QFloat32(mantissa, exponent) \
  { \
    Fl2Fract16(mantissa), \
    Fl2Int16(exponent) \
  }

// *****************************************************************************
/* Function Exponent16ToQFloat32
  
   Summary:
     Converts a power of 2 16-bit integer to 32-bit floating point value.

   Description:
     Converts a power of 2 16-bit integer to 32-bit floating point value.
  
   Parameters:
     int16_t - 16-bit integer number for power (expected to be a variable).
  
   Returns:
     32-bit floating point value of 2^n.
  
*/
#define Exponent16ToQFloat32(value) \
( \
  (((value) < 0) ? ((float)1.0/(float)(1UL<<(-(value)))): \
   ((float)(1UL<<(value)))) \
)


// *****************************************************************************
/* Function Fx16Norm:
  
   Summary:
     Normalize the 16-bit fractional value. 

   Description:
     Produces then number of left shifts needed to Normalize the 16-bit
     fully fractional input.  If the input 'a' is a positive number, it will produce
     the number of left shifts required to normalized it to the range of a
     minimum of [(MAXFRACT16+1)/2] to a maximum of [MAXFRACT16].
     If the input 'a' is a negative number, it will produce
     the number of left shifts required to normalized it to the range of a
     minimum of [MINFRACT16] to a maximum of [MINFRACT16/2].
     This function does not actually normalize the input, it just produces
     the number of left shifts required.  To actually normalize the value
     the left shift function should be used with the value returned From
     this function.

     the 16-bit input on range: 0 => result < 16  (i.e. NUMBITSFRACT16)
                  If a>0: 0x4000 > Normalized Value <= 0x7fff
                     i.e.  (MAXFRACT16+1)/2 >  aNorm <= MAXFRACT16
                  If a<0: 0x8000 >= Normalilzed Value < 0xC000
                     i.e.  MINFRACT16>=  aNorm < MINFRACT16/2

     This function relates to the ETSI norm_s function.
  
   Parameters:
     q15 - a in Q1.15 
  
   Returns:
     i16 result - The number of left shifts required to normalize 
  
*/
int16_t Fx16Norm(q15);      


// *****************************************************************************
/* Function Fx32Norm:
  
   Summary:
     Normalize the 32-bit number.

   Description:
     Produces then number of left shifts needed to Normalize the 32-bit
     fractional input.  If the input 'a' is a positive number, it will produce
     the number of left shifts required to normalized it to the range of a
     minimum of [(MAXFRACT32+1)/2] to a maximum of [MAXFRACT32].
     If the input 'a' is a negative number, it will produce
     the number of left shifts required to normalized it to the range of a
     minimum of [MINFRACT32] to a maximum of [MINFRACT32/2].
     This function does not actually normalize the input, it just produces
     the number of left shifts required.  To actually normalize the value
     the left-shift function should be used with the value returned From
     this function.

     32-bit input on range: 0 => result < 32  (i.e. NUMBITSFRACT32)
                  If a>0: 0x40000000 > Normalized Value <= 0x7fffffff
                     i.e.  (MAXFRACT32+1)/2 >  aNorm <= MAXFRACT32
                  If a<0: 0x80000000 >= Normalized Value < 0xC0000000
                     i.e.  MINFRACT32>=  aNorm < MINFRACT32/2
  
     This function relates to the ETSI norm_l function.
  
   Parameters:
     q31 a   - 32-bit Q1.d31 to be normalized
  
   Returns:
     int16_t result - The number of left shifts required to normalize the 
*/
int16_t Fx32Norm(q31);                 

// *****************************************************************************
/* Function:
    fract_q15d16_t Fract_Convert_Q15bfpToQ15d16( fract_q15bfp_t x )

  Summary:
    Convert Q15 block floating point to Q15.16 fixed point.

  Description:
    Convert Q15 block floating point to Q15.16 fixed point.

  Precondition:
    None.

  Parameters:
    x - Q15 Block Floating Point

  Returns:
    Input converted to Q15.16 fixed point.

  Example:
  <code>
    fract_q15d16_t x_Q15d16;
    fract_q15bfp_t x_Q15bfp;

    x_Q15bfp.m = 0x4000; // 1/2
    x_Q15bfp.e  = -1;    // 1/2>>1 = 1/4

    x_Q15d16 = Fract_Convert_Q15bfpToQ15d16( x_Q15bfp );
    // x_Q15d16 = 0x0000 4000
  </code>

  Remarks:
    No effort is made to trap overflows of underflows,
*/
fract_q15d16_t Fract_Convert_Q15bfpToQ15d16( fract_q15bfp_t x );

// *****************************************************************************
/* Function:
    fract_q31_t Fract_Convert_Q15bfpToQ31( fract_q15bfp_t x )

  Summary:
    Convert Q15 block floating point to Q31 fixed point.

  Description:
    Convert Q15 block floating point to Q31 fixed point.

  Precondition:
    None.

  Parameters:
    x - Q15 Block Floating Point

  Returns:
    Input converted to Q31 fixed point.

  Example:
  <code>
  </code>

  Remarks:
    No effort is made to trap overflows of underflows,
*/
fract_q31_t Fract_Convert_Q15bfpToQ31( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_Convert_Q15ToQ15bfp( fract_q15_t x )

  Summary:
    Convert from Q15 fixed point to Q15 Block Floating Point

  Description:
    Convert from Q15 fixed point to Q15 Block Floating Point

  Precondition:
    None.

  Parameters:
    x - Q15 (aka Q1.15) fixed point

  Returns:
    Input convertd to Q15 Block Floating Point.

  Example:
  <code>
    fract_q15_t    x_Q15;
    fract_q15bfp_t x_Q15bfp;

    x_Q15    = 0xE000; //-1/4
    x_Q15bfp = Fract_Convert_Q15d16ToQ15bfp(x_Q15d16);
    //x_Q15bfp.m = 0xC000  // -1/2
    //x_Q15bfp.e = 0xFFFF = -1; // -1/2>>1 = -1/4
  </code>

  Remarks:
    Mantissa and exponent are adjusted to remove redundant leading bits.
*/
fract_q15bfp_t Fract_Convert_Q15ToQ15bfp( fract_q15_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_Convert_Q15d16ToQ15bfp( fract_q15d16_t x )

  Summary:
    Convert from Q15.16 fixed point to Q15 Block Floating Point

  Description:
    Convert from Q15.16 fixed point to Q15 Block Floating Point

  Precondition:
    None.

  Parameters:
    x - Q15.16 fixed point

  Returns:
    Input convertd to Q15 Block Floating Point.

  Example:
  <code>
    fract_q15d16_t x_Q15d16;
    fract_q15bfp_t x_Q15bfp;

    x_Q15d16 = 0xFFFFC000; //-1/4
    x_Q15bfp = Fract_Convert_Q15d16ToQ15bfp(x_Q15d16);
    //x_Q15bfp.m = 0xC000  // -1/2
    //x_Q15bfp.e = 0xFFFF = -1; // -1/2>>1 = -1/4
  </code>

  Remarks:
    Mantissa and exponent are adjusted to remove redundant leading bits.
*/
fract_q15bfp_t Fract_Convert_Q15d16ToQ15bfp( fract_q15d16_t x );



// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/*  LibQ - Optimized fixed point math library
*/

// *****************************************************************************
/* Function libq_q1d15_Sin_q10d6:
  
   Summary:
     Approximates the sine of an angle. 

   Description:
     This function approximates the sine of an angle using the
     following algorithm: sin(x) = 3.140625x + 0.02026367x^2 -
     5.325196x^3 + 0.5446778x^4 + 1.800293x^5.  The approximation
     is accurate for any value of x from 0 degrees to 90 degrees.
     Because sin(-x) = - sin(x) and sin(x) = sin(180 - x), the sine
     of any angle can be inferred from an angle in the first quadrant.
     Therefore, any angle > 90 is converted to an angle between 0 & 90.
     The coefficients of the algorithm have been scaled by 1/8 to
     fit a Q1d15 format.  So the result is scaled up by 8 to obtain
     the proper magnitudes.  The algorithm expects the angle to be in
     degrees and represented in Q10.6 format.  The computed sine value
     is returned in Q1.15 format.

   Precondition:
     none.
  
   Parameters:
     q15 angle - The angle in degrees for which the sine is computed in Q10.6
  
   Returns:
     q15 sine(angle) value in Q1.15

*/
q15 libq_q1d15_Sin_q10d6(q15 angleQ10d6);


// *****************************************************************************
/* Function libq_q20d12_Sin_q20d12:
  
   Summary:
     3rd order Polynomial apprx. of a sine function

   Description:
     3rd order Polynomial apprx. of a sine function

   Precondition:
     none.
  
   Parameters:
     q31 angle - The angle in radians for which the sine is computed in Q20.12
  
   Returns:
     q31 Sine(angle) value in Q20.12 

*/
q31 libq_q20d12_Sin_q20d12(q31 angQ20d12); 


  
// *****************************************************************************
/* Function libq_q15_Abs_q15:
  
   Summary:
     Saturated Absolute value.

   Description:
     Creates a saturated Absolute value.  It takes the absolute value of the
     16-bit 2s-complement fractional input with saturation. The saturation is
     for handling the case where taking the absolute value of MINFRACT16 is
     greater than MAXFRACT16, or the allowable range of 16-bit values.
     This function relates to the ETSI abs function.
  
   Parameters:
     q15 a - input argument
  
   Returns:
     q15 result - abs(input) <= MAXFRACT16
       
*/
q15 libq_q15_Abs_q15(q15);   

  
// *****************************************************************************
/* Function libq_q31_Abs_q31:
  
   Summary:
     Saturated Absolute value.

   Description:
     Creates a saturated Absolute value.  It takes the absolute value of the
     32-bit 2s-complement fractional input with saturation. The saturation is
     for handling the case where taking the absolute value of MINFRACT32 is
     greater than MAXFRACT32, or the allowable range of 32-bit values.

     This function relates to the ETSI L_abs function.
  
   Parameters:
     q31 a 
  
   Returns:
     q31 result -  abs(a) <= MAXFRACT32
*/
  
q31 libq_q31_Abs_q31(q31);   


// *****************************************************************************
/* Function libq_q15_Negate_q15:
 
   Summary:
     Negate 16-bit 2s-complement fractional value with saturation. 
  
   Description:
     Negate 16-bit 2s-complement fractional value with saturation. The
     saturation is for handling the case where negating a MINFRACT16 is
     greater than MAXFRACT16, or the allowable range of values.
     This function relates to the ETSI negate function.
  
   Parameters:
     q15 a
  
   Returns:
     q15 result on range: MINFRACT16 <= result <= MAXFRACT16
  
*/
q15 libq_q15_Negate_q15(q15);    


// *****************************************************************************
/* Function libq_q31_Negate_q31:
 
   Summary:
     Negate 32-bit 2s-complement fractional value with saturation.
  
   Description:
     Negate 32-bit 2s-complement fractional value with saturation. The
     saturation is for handling the case where negating a MINFRACT32 is
     greater than MAXFRACT32, or the allowable range of values.
     This function relates to the ETSI L_negate function.
  
   Parameters:
     q31 a 
  
   Returns:
     q31 result on range: MINFRACT32 <= result <= MAXFRACT32

*/
q31 libq_q31_Negate_q31(q31);                    


// *****************************************************************************
/* Function libq_q15_RoundL_q31:
  
   Summary:
     Rounds the lower 16-bits of the 32-bit fractional input.

   Description:
     Rounds the lower 16-bits of the 32-bit fractional input into a 16-bit
     fractional value with saturation. This converts the 32-bit fractional
     value to 16-bit fractional value with rounding.  This function calls the
     'Add' function to perform the 32-bit rounding of the input value and
     'ExtractH' function to extract to top 16-bits.  This has the effect of
     rounding positive fractional values up and more positive, and has the
     effect of rounding negative fractional values up and more positive.
     This function relates to the ETSI round function.
  
   Parameters:
     q31 a 
  
   Returns:
     q15 result


*/
q15 libq_q15_RoundL_q31(q31);        


// *****************************************************************************
/* Function libq_q31_DepositH_q15:


   Summary:
     Place 16 bits in the upper half of 32 bit word. 
  
   Description:
     Composes a 32-bit fractional value by placing the input 16-bit fractional
     value in the composite MSB's and zeros the composite 16-bit LSB's
     This is a bit-for-bit placement of input 16-bits into the upper part
     of 32-bit result.

     This function relates to the ETSI L_deposit_H function.
  
   Parameters:
     q15 a
  
   Returns:
     q31 result 16-bits of a in upper MSB's and zeros in the lower LSB's

   Remarks:

*/
q31 libq_q31_DepositH_q15(q15);  


// *****************************************************************************
/* Function libq_q31_DepositL_q15:
  
   Summary:
     Place 16 bits in the lower half of 32 bit word. 

   Description:
     Composes a 32-bit fractional value by placing the 16-bit Fraction input
     value into the lower 16-bits of the 32-bit composite value. The 16-bit
     MSB's of the composite output are sign extended. This is a bit-for-bit
     placement of input 16-bits into the bottom portion of the composite
     32-bit result with sign extention.
     This function relates to the ETSI L_deposit_l function.
  
   Parameters:
     q15 a 
  
   Returns:
     q31 result - SignExtended 16-bit MSB's and a Value in lower 16-bit LSB's

*/
q31 libq_q31_DepositL_q15(q15);     


// *****************************************************************************
/* Function libq_q15_ExtractH_q31:
  
   Summary:
     Extracts upper 16 bits of input 32-bit fractional value. 


   Description:
     Extracts upper 16 bits of input 32-bit fractional value and returns them
     as 16-bit fractional value.  This is a bit-for-bit extraction of the top
     16-bits of the 32-bit input.
     This function relates to the ETSI extract_h function.
  
   Parameters:
     q31 a
  
   Returns:
     q15 result - Upper 16 bits of 32-bit argument a

   Remarks:

*/
q15 libq_q15_ExtractH_q31(q31);


// *****************************************************************************
/* Function libq_q15_ExtractL_q31:
  
   Summary:
     Extracts lower 16-bits of input 32-bit fractional value.

   Descriptionf
     Extracts lower 16-bits of input 32-bit fractional value and returns them
     as 16-bit fractional value.  This is a bit-for-bit extraction of the
     bottom 16-bits of the 32-bit input.
     This function relates to the ETSI extract_l function.
  
   Parameters:
     q31 a 
  
   Returns:
     q15 - Lower 16 bits of 32-bit argument a

*/
q15 libq_q15_ExtractL_q31(q31); 


// *****************************************************************************
/* Function libq_q15_Add_q15_q15:
f
   Summary:
     Add two 16-bit 2s-complement fractional values. 
  
   Description:
     Add two 16-bit 2s-complement fractional (op1 + op2) to produce a 16-bit
     2s-complement fractional result with saturation. The saturation is for
     handling the overflow/underflow cases, where the result is set to
     MAX16 when an overflow occurs and the result is set to
     MIN16 when an underflow occurs.  This function does not produce
     any status flag to indicate when an overflow or underflow has occured.
     It is assumed that the binary point is in exactly the same bit position
     for both 16-bit inputs and the resulting 16-bit output.
  
   Parameters:
     q15 a 
     q15 b
  
   Returns:
     q15 - a+b on Range: MINFRACT16 <= result <= MAXFRACT16
*/
q15 libq_q15_Add_q15_q15(q15, q15);                


// *****************************************************************************
/* Function libq_q31_Add_q31_q31:
 
   Summary:
     Add two 32-bit 2s-complement fractional values.

   Description:
     Add two 32-bit 2s-complement fractional (op1 + op2) to produce a 32-bit
     2s-complement fractional result with saturation. The saturation is for
     handling the overflow/underflow cases, where the result is set to
     MAX32 when an overflow occurs and the result is set to
     MIN32 when an underflow occurs.  This function does not produce
     any status flag to indicate when an overflow or underflow has occured.
     It is assumed that the binary point is in exactly the same bit position
     for both 32-bit inputs and the resulting 32-bit output.
     This function relates to the ETSI L_add function.
  
   Parameters:
     q31 a 
     q31 b
  
   Returns: 
     q31 result a+b on range: MINFRACT32 <= result <= MAXFRACT32
*/
q31 libq_q31_Add_q31_q31(q31, q31);             


// *****************************************************************************
/* Function libq_q15_Sub_q15_q15:
  
   Summary:
     Subtract two 16-bit 2s-complement fractional values

   Description:
     Subtract two 16-bit 2s-complement fractional (op1 - op2) to produce a
     16-bit 2s-complement fractional difference result with saturation. The
     saturation is for handling the overflow/underflow cases, where the result
     is set to MAX16 when an overflow occurs and the result is set to
     MIN16 when an underflow occurs.  This function does not produce
     any status flag to indicate when an overflow or underflow has occured.
     It is assumed that the binary point is in exactly the same bit position
     for both 16-bit inputs and the resulting 16-bit output.
     This function relates to the ETSI sub function.
  
  
   Parameters:
     q15 a
     i16 b
  
   Returns:
     q15 result a+b on range: MINFRACT16 <= result <= MAXFRACT16
  
*/
q15 libq_q15_Sub_q15_q15(q15, q15);                


// *****************************************************************************
/* Function libq_q31_Sub_q31_q31:
  
   Summary:
     Subtract two 32-bit 2s-complement fractional values

   Description:
     Subtract two 32-bit 2s-complement fractional (op1 - op2) to produce a
     16-bit 2s-complement fractional difference result with saturation. The
     saturation is for handling the overflow/underflow cases, where the result
     is set to MAX32 when an overflow occurs and the result is set to
     MIN32 when an underflow occurs.  This function does not produce
     any status flag to indicate when an overflow or underflow has occured.
     It is assumed that the binary point is in exactly the same bit position
     for both 16-bit inputs and the resulting 16-bit output.
     This function relates to the ETSI sub function.
  
  
   Parameters:
     q31 a
     q31 b
  
   Returns:
     q31 result a-b on range: MINFRACT31 <= result <= MAXFRACT31
  
*/
q31 libq_q31_Sub_q31_q31(q31, q31);


// *****************************************************************************
/* Function libq_q15_ShiftLeft_q15_i16:
 
   Summary:
     'Arithmetic' Shift of the 16-bit input argument. 
  
   Description:
     Performs an 'Arithmetic' Shift of the 16-bit input argument 'a' left
     by the input argument 'b' bit positions.  If 'b' is a positive number,
     a 16-bit left shift is performed with 'zeros' inserted to the right
     of the shifted bits.  If 'b' is a negative number, a right shift by abs(b)
     positions with 'sign extention' 
     Saturation is applied if shifting causes an overflow or an underflow.

     positive value: # of bits to left shift (zeros inserted at LSB's)
     {To not always saturate: if 'a=0', then max b=15, else max b=14}
     negative value: # of bits to right shift (sign extend)

     This function relates to the ETSI shl function.
  
   Parameters:
     q15 a - 16-bit signed integer value to be shifted.  
     i16 b - 16-bit signed integer shift value
  
   Returns:
     q15 result - arithmetically shifted 16-bit signed integer output
  
*/
q15 libq_q15_ShiftLeft_q15_i16(q15, i16);   


// *****************************************************************************
/* Function libq_q31_ShiftLeft_q31_i16:
  
   Summary:
     'Arithmetic' Shift of the 32-bit value.

   Description:
     Performs an 'Arithmetic' Shift of the 32-bit input argument 'a' left
     by the input argument 'b' bit positions.  If 'b' is a positive number,
     a 32-bit left shift is performed with 'zeros' inserted to the right
     of the shifted bits.  If 'b' is a negative number, a 32-bit right shift
     by b bit positions with 'sign extention': 

       positive value: # of bits to left shift (zeros inserted at LSB's)
       negative value: # of bits to right shift (sign extend)

     Saturation is applied if shifting causes an overflow or an
     underflow.

     This function relates to the ETSI L_shl function.
  
   Parameters:
     q31 a - 32-bit signed integer value to be shifted
     i16   - 16-bit signed integer shift index
  
   Returns:
     q31 result - arithmetically shifted 32-bit signed integer output
  
*/
q31 libq_q31_ShiftLeft_q31_i16(q31, i16);  


// *****************************************************************************
/* Function libq_q15_ShiftRight_q15_q15:
  
   Summary:
     'Arithmetic' RIGHT Shift on a 16-bit value.

   Description:
     Performs an 'Arithmetic' RIGHT Shift on a 16-bit input by 'b' bit
     positions.  For positive shift directions (b>0), 'b' Lsb-bits are shifted
     out to the right and 'b' sign-extended Msb-bits fill in From the left.
     For negative shift directions (b<0), 'b' Lsb's are shifted to the
     LEFT with 0's filling in the empty lsb position. The left shifting
     causes 'b' Msb-bits to fall off to the left, saturation is applied to
     any shift left value that overflows. This function calls the
     left-shift function to perform any 16-bit left shifts.  This function
     does not provide any status-type information to indicate when overflows
     occur.

        positive value: # of bits to right shift (sign extend)
                        { To get all sign bits, b>=15 }
        negative value: # of bits to left shift (zeros inserted at LSB's)

     This function relates to the ETSI shr function.
  
   Parameters:
     q15 a - 16-bit signed input value to shift
     i16 b - 16-bit signed integer shift index
  
   Returns:
     q15 result - 16-bit signed shifted output
  
*/
q15 libq_q15_ShiftRight_q15_i16(q15, i16);    


// *****************************************************************************
/* Function libq_q31_ShiftRight_q31_q15:
  
   Summary:
     'Arithmetic' RIGHT Shift on a 32-bit value.

   Description:
     Performs an 'Arithmetic' RIGHT Shift on a 32-bit input by 'b' bit
     positions.  For positive shift directions (b>0), 'b' Lsb-bits are shifted
     out to the right and 'b' sign-extended Msb-bits fill in From the left.
     For negative shift directions (b<0), 'b' Lsb's are shifted to the
     LEFT with 0's filling in the empty lsb position. The left shifting
     causes 'b' Msb-bits to fall off to the left, saturation is applied to
     any shift left value that overflows. This function calls the
     left-shift function to perform any 32-bit left shifts.  This function
     does not provide any status-type information to indicate when overflows
     occur.

       positive value: # of bits to right shift (sign extend)
       negative value: # of bits to left shift (zeros inserted at LSB's)

     This function relates to the ETSI L_shr function.
  
   Parameters:
     q31 a - 32-bit signed integer value to be shifted
     i16 b - 16-bit signed integer shift index
  
   Returns:
     q31 result - Arithmetically shifted 32-bit signed integer output
  
*/
q31 libq_q31_ShiftRight_q31_i16(q31, i16); 


// *****************************************************************************
/* Function libq_q15_ShiftRightR_q15_q15:
  
   Summary:
     Performs an 'Arithmetic' RIGHT Shift on a 16-bit input.
  
   Description:
     Performs an 'Arithmetic' RIGHT Shift on a 16-bit input by 'b' bits with
     Rounding applied.  The rounding occurs by adding a bit weight of "1/2 Lsb",
     where the "Lsb" is the Ending (shifted) Lsb. For example: The initial Bit#(b)
     is after the right shift Bit#(0), so the rounding bit weight is Bit#(b-1).
     Rounding does not occur on either left shifts or on no shift needed cases.
     For positive shift directions (b>0), 'b' Lsb-bits are shifted out
     to the right and 'b' sign-extended Msb-bits fill in From the left.
     For negative shift directions (b<0), 'b' Lsb's are shifted to the LEFT
     with 0's filling in the empty lsb position. The left shifting causes
     'b' Msb-bits to fall off to the left, saturation is applied to any shift
     left value that overflows. This function calls the left-shift function
     to perform the actual 16-bit left shift.  This function does not provide
     any status-type information to indicate when overflows occur.

        positive value: # of bits to right shift (sign extend)
                       {b > 15, results in all sign bits}
        negative value: # of bits to left shift (zeros inserted at LSB's)
  
     This function relates to the ETSI shr_r function.
  
   Parameters:
     q15 a - 16-bit signed integer value to be shifted
     i16 b - 16-bit signed integer shift index
  
   Returns:
     q15 result - Arithmetically shifted 16-bit signed integer output
  
*/
q15 libq_q15_ShiftRightR_q15_i16(q15, i16);                  


// *****************************************************************************
/* Function libq_q31_ShiftRightR_q31_q15:
  
   Summary:
     'Arithmetic' RIGHT Shift on a 32-bit value

   Description:
     Performs an 'Arithmetic' RIGHT Shift on a 32-bit input by 'b' bits with
     Rounding applied.  The rounding occurs before any shift by adding a
     bit weight of "1/2 Lsb", where the "Lsb" is the Ending (shifted) Lsb.
     For example: The initial Bit#(i+b) is after the right shift Bit#(i),
     so the rounding bit weight is Bit#(i+b-1).  Rounding does not occur
     on left shifts, when b is negative.  After rounding, this function
     calls the right-shift function to perform the actual 32-bit right shift.
     For positive shift directions (b>0), 'b' Lsb-bits are shifted out
     to the right and 'b' sign-extended Msb-bits fill in From the left.
     For negative shift directions (b<0), 'b' Lsb's are shifted to the LEFT
     with 0's filling in the empty lsb position. The left shifting causes
     'b' Msb-bits to fall off to the left, saturation is applied to any shift
     left value that overflows. This function calls the left-shift function
     to perform the actual 32-bit left shift.  This function does not provide
     any status-type flag to indicate occurence of overflow.

       positive value: # of bits to right shift (sign extend)
       negative value: # of bits to left shift (zeros inserted at LSB's)

     This function relates to the ETSI L_shr_r function.
  
   Parameters:
     q31 a - 32-bit signed integer value to be shifted
     i16 b - 16-bit signed integer shift index
  
   Returns:
     q31 result - Arithmetically shifted 32-bit signed integer output
  
*/
q31 libq_q31_ShiftRightR_q31_i16(q31, i16);                  


// *****************************************************************************
/* Function libq_q31_Mult2_q15_q15:
  
   Summary:
     fractional multiplication of two 16-bit fractional values.

   Description:
     Performs fractional multiplication of two 16-bit fractional values
     and returns the 32-bit fractional scaled result. The function
     performs the Q15xQ15->Q30 fractional bit multiply.  It then shifts the
     result left by '1', to give a Q31 type result, (the lsb is zero-filled).
     This automatic shift left is done to get rid of the extra sign bit
     that occurs in the interpretation of the fractional multiply result.
     Saturation is applied to any results that overflow, and then the
     function returns the 32-bit fractional q31 result.  This function is
     for fractional 'Q' data only and it therefore will not give
     correct results for true integers (because left shift by '1').
     For the special case where both inputs equal the MINFRACT16, the
     function returns a value equal to MAXFACT32,
     i.e. 0x7fffffff = libq_q15_mult_q15_q31(0x8000,0x8000).

     This function relates to the ETSI L_mult function.
  
   Parameters:
     q15 a - multiplicand a 
     q15 b - multiplicand b
  
   Returns:
     q31, a*b 
*/
q31 libq_q31_Mult2_q15_q15(q15, q15);           


// *****************************************************************************
/* Function libq_q15_MultiplyR2_q15_q15:
  
   Summary:
     fractional multiplication of two 16-bit fractional values giving a 16 
     bit rounded result.

   Description:
     Performs fractional multiplication of two 16-bit fractional values
     and returns a ROUNDED 16-bit fractional result. The function performs
     a Q15xQ15->Q30 bit multiply with a left shift by '1' to give a Q31
     result.  This automatic shift left is done to get rid of the extra sign
     bit that occurs in the interpretation of the fractional multiply result.
     Saturation is applied to any 32-bit result that overflows. Rounding
     is applied to the 32-bit SHIFTED result by adding in a weight factor
     of 2^15, again any overflows are saturated.  The TOP 16-bits are
     extracted and returned.  This function is for fractional 'Qtype' data
     only and it therefore will not give the correct results for
     true integers (because left shift by '1').  This function assumes that
     the binary point in the 32-bit shifted multiplier output is between
     bit_16 and bit_15 when the rounding factor is added. For the special
     case where both inputs equal the MINFACT16, the function returns a
     value equal to MAXFACT16, i.e. 0x7fff = 'libq_q15_Mult2_q15_q15(0x8000,0x8000)'.
     This function internally calls the libq_q15_mult_q15_q31() routine to 
     perform the actual multiplication and the rounding routine to perform the
     actual rounding.

     This function relates to the ETSI mult_r function.
  
   Parameters:
     q15 a - value in Q1.15
     q15 b - value in Q1.15
  
   Returns:
     q15 result - a*b rounded 16-bit signed integer (Q1.15) output value
  
*/
q15 libq_q15_MultiplyR2_q15_q15(q15, q15);

//DEPRECATED -- mis-spelled
#define libq_q15_MultipyR2_q15_q15(a,b)  libq_q15_MultiplyR2_q15_q15(a,b)

// *****************************************************************************
/* Function libq_q31_Multi_q15_q31():
  
   Summary:
     Implement 16 bit by 32 bit multiply.

   Description:
     Implement 16 bit by 32 bit multiply as shown below 
     The 's' and 'u' notation shows the processing of signed and 
     unsigned numbers.
  
          -B1- -B0-   s u     2nd argument is 32 bits
               -A0-     s     1st argument is 16 bits
     --------------
          A0B0 A0B0   s=s*u   1st 32-bit product is A0*B0
     A0B1 A0B1        s=s*s   2nd 32-bit product is A0*B1
     --------------
     -S2- -S1- -S0-   s=s+s   48-bit result is the sum of products
     -P1- -P0-                32-bit return is the most significant bits of sum
  
     The algorithm is implemented entirely with the fractional arithmetic
     library. The unsigned by signed multiply is implemented by shifting bits
     15:1 to  bits 14:0 of a 16-bit positive fractional number, which throws
     away bit 0 of the 32-bit number. Since that affects result bits that are
     used for rounding, rounding processing is included. Saturation processing
     is handled implicitly in the fractional arithmetic library, except for the
     case of maximum negative numbers.
  
   Parameters:
     q15 a 
     Q31 b 
  
   Returns:
     q31 result - a*b rounded
  
  
*/
q31 libq_q31_Multi_q15_q31(q15 argAQ1d15, q31 argBQ1d31);


// *****************************************************************************
/* Function libq_q15_DivisionWithSaturation_q15_q15():

   Summary:
     Fractional division with saturation.

   Description:
     Performs fractional division with saturation. There are three restrictions
     that the calling code must satisfy.
     1. Both the numerator and denominator must be positive.
     2. In order to obtain a non-saturated result, the numerator must be LESS
        than or equal to the denominator.
     3. The denominator must not equal zero.
     If 'num' equals 'den', then the result equals MAXINT16.

     This function relates to the ETSI div_s function.
  
   Parameters:
     q15 num - 16-bit fractional numerator
     q15 den - 16-bit fractional denumerator
  
   Returns:
     q15 result - ratio a/b in 16-bit fractional format
  
*/
q15 libq_q15_DivisionWithSaturation_q15_q15(q15, q15);      


// *****************************************************************************
/* Function libq_q31_Mac_q31_q15_q15():
 
   Summary:
     Multiply-Accumulate function WITH saturation
  
   Description:
     Performs a Multiply-Accumulate function WITH saturation.  This routine
     returns the fully fractional 32-bit result From the accumulator output
     'SAT(addOut_Q1d31)=outQ1d15' where 'multOut_Q1d31 + a_Q1d31 = addOut_Q1d31',
     and 'b_Q1d15 x c_Q1d15 = multOut_Q1d31'. The multiply is performed on the
     two 16-bit fully fully fractional input values 'b x c' which results in a 32-bit value.
     This result is left shifted by one to account for the extra sign bit
     inherent in the fully fully fractional-type multiply.  The shifted number represents
     a Q1d31 number with the lsb set to '0'.  This Q1d31 number is added with
     the 32-bit fully fully fractional input argument 'a'.  Saturation is applied on the
     output of the accumulator to keep the value within the 32-bit fully fractional
     range and then this value is returned.
     This function is for fully fractional Q-type format data only and it
     therefore will not give the correct results for true integers.

     This function relates to the ETSI L_mac function.
  
   Parameters:
     q31 a -  32-bit accumulator operand 1 in Q1d31
     q15 b -  16-bit multiplication operand 1 in Q1d15
     q15 c -  16-bit multiplication operand 2 in Q1d15
  
   Returns:
     q31 result, a+b*c saturated
  
*/
q31 libq_q31_Mac_q31_q15_q15(q31, q15, q15);    


// *****************************************************************************
/* Function libq_q15_MacR_q31_q15_q15:
  
   Summary:
     Multiply accumulate with rounding.


   Description:
     This function is multiply-accumulate WITH Rounding applied to the
     accumulator result before it is saturated and the top 16-bits taken.
     This function first multiplies the two 16-bit input values 'b x c' which
     results in a 32-bit value.  This result is left shifted by one to account
     for the extra sign bit inherent in the fractional-type multiply. So, the
     shifted number now has a '0' in the Lsb. The shifted multiplier output is
     then added to the 32-bit fractional input 'a'. Then the 32-bits of the
     accumulator output are rounded by adding '2^15'.  This value is then
     saturated to be within the q15 range.
     It is assumed that the binary point of the 32-bit input value a is in
     the same bit position as the shifted multiplier output.
     This function is for fractional Qtype format data only and
     it therefore will not give the correct results for true integers.
   
     This function relates to the ETSI L_mac_r function.
  
   Parameters:
     q31 a -  32-bit accumulator operand 
     q15 b -  16-bit multiplication operand 
     q15 c -  16-bit multiplication operand
  
   Returns:
     q15 result - a+b*c rounded
  
*/
q15 libq_q15_MacR_q31_q15_q15(q31, q15, q15);


// *****************************************************************************
/* Function libq_q31_Msu_q31_q15_q15:
  
   Summary:
    Multiply-Subtraction with saturation

   Description:
     Performs a Multiply-Subtraction function WITH saturation.  This routine
     returns the fractional 32-bit result From the subtractor output
     'SAT(addOut_Q1d31)=outQ1d15' where 'a_Q1d31 - multOut_Q1d31 = addOut_Q1d31',
     and 'b_Q1d15 x c_Q1d15 = multOut_Q1d31'. The multiply is performed on the
     two 16-bit fractional input values 'b x c' which results in a 32-bit value.
     This result is left shifted by one to account for the extra sign bit
     inherent in the fractional-type multiply.  The shifted number represents a
     Q1d31 number with the lsb set to '0'.  This Q1d31 number then is subtracted
     From the 32-bit fractional input argument 'a'.  Saturation is
     applied on the output of the accumulator(Subtractor) to keep the value
     within the 32-bit fractional range and then this value is returned.

     This function is for fractional Q-type format data only and it
     therefore will not give the correct results for true integers.

     This function relates to the ETSI L_msu function.
  
   Parameters:
     q31 a - 32-bit operand (Q1d31)that is subtracted From
     q15 b - 16-bit (Q1d15) multiplication operand 1
     q15 c - 16-bit (Q1d15) multiplication operand 2
  
   Returns:
     q31 result - a-b*c saturated to Q1.31 
  
*/
q31 libq_q31_Msu_q31_q15_q15(q31, q15, q15);    /* L_msu(a,b,c)  */


// *****************************************************************************
/* Function libq_q15_MsuR_q31_q15_q15:
  
   Summary:
    Multiply-Subtraction with rounding 

   Description:
     This function is like Multiply-Subtract but WITH Rounding applied to the
     subtractor result before it is saturated and the top 16-bits taken.
     This function first multiplies the two 16-bit input values 'b x c' which
     results in a 32-bit value.  This result is left shifted by one to account
     for the extra sign bit inherent in the fractional-type multiply. So, the
     shifted number now has a '0' in the Lsb. The shifted multiplier output
     is then SUBTRACTED From the 32-bit fractional input 'a'. Then the 32-bits
     output From this subtraction are rounded by adding '2^15'.  This value
     is then saturated to be within the q15 range.
     It is assumed that the binary point of the 32-bit input value a is in
     the same bit position as the shifted multiplier output.

     This function is for fractional Q-type format data only and
     it therefore will not give the correct results for true integers.

     This function relates to the ETSI msu_r function.
  
   Parameters:
     q31 a - Value which is subtracted from  
     q15 b - multiplication operand 1
     q15 c - multiplication operand 2
  
   Returns:
     q15 result - a-b*c rounded to Q1.15
  
*/
q15 libq_q15_MsuR_q31_q15_q15(q31, q15, q15);  


// *****************************************************************************
/* Function libq_q15_ExpAvg_q15_q15_q1d15()
  
   Summary:
     Exponential averaging 

   Description:
     Exponential averaging implements a smoothing function based on the form:
         avg[i+1] = avg[i] * lamda + new * (1-lamda)
     In this implementation, is has been optimized as follows.
         avg[i+1] = (avg[i] - new) * lamda + new
  
     The optimization precludes accurate processing of new numbers that differ
     from the current average by more than unity. If the difference is greater
     than unity or less than negative unity, the difference is saturated.
  
     The effect is akin to a smaller lambda, e.g., the new value will have a
     greater weight than expected. If the smoothing is of data that is entirely
     positive or entirely negative, then the saturation will not be an issue.
  
   Parameters:

     q15 S(k) - Previous exponential average
     q15 X(k)-  New value to be averaged in
     q15 L -  exponential averaging constant in Q1.15
  
   Returns:
     q15 result - S(k+1) = S(k)*L + X(k)*(1-L) 
  
*/
q15 libq_q15_ExpAvg_q15_q15_q1d15(q15 prevAvgQ15,
                                  q15 newMeasQ15,
                                  q15 lamdaQ1d15);


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_XplusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y )

  Summary:
    Add two Q15 block floating point numbers.

  Description:
    Add two Q15 block floating point numbers.

  Precondition:
    none.

  Parameters:
    x - Q15 block floating point term
    y - Q15 block floating point term

  Returns:
    x+y as Q15 block floating point

  Example:
  <code>
    fract_q15d16_t x, y;
    fract_q15bfp_t X, Y, XplusY;

    x = 0x00030000; // 3
    y = 0x00070000; // 7
    X = Fract_Convert_Q15d16ToQ15bfp(x);
    Y = Fract_Convert_Q15d16ToQ15bfp(y);
    XplusY = Fract_XplusY_Q15bfp( X, Y );
    // XplusY.m = 0x5000 = 20480/32768 = 0.625
    // XPlusY.e = 4
    // XPlusY = 0.625<<4 = 10
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_XplusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_XminusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y )

  Summary:
    Subtract two Q15 block floating point numbers.

  Description:
    Subtract two Q15 block floating point numbers.

  Precondition:
    none.

  Parameters:
    x - Q15 block floating point term
    y - Q15 block floating point term

  Returns:
    x-y as Q15 block floating point

  Example:
  <code>
    fract_q15d16_t x, y;
    fract_q15bfp_t X, Y, XminusY;

    x = 0x00030000; // 3
    y = 0x00070000; // 7
    X = Fract_Convert_Q15d16ToQ15bfp(x);
    Y = Fract_Convert_Q15d16ToQ15bfp(y);
    XminusY = Fract_XminusY_Q15bfp( X, Y );
    // XplusY.m = 0xC000 = -16384/32768 = -0.5
    // XPlusY.e = 3
    // XPlusY = -0.5<<3 = -4
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_XminusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
// *****************************************************************************
// Section: Multiply and Divide Operations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    fract_q15_t Fract_XtimesY_Q15(fract_q15_t x, fract_q15_t y );

  Summary:
    Calculates product of X*Y for Q15 fixed point

  Description:
    Calculates product of X*Y for Q15 fixed point

  Precondition:
    None

  Parameters:
    x - Q15 multiplicand
    y - Q15 multiplicand

  Returns:
    Q15 product x*y

  Example:
  <code>
    fract_q15_t x, y, xtimesy;

    x = 0x1234; //  4660/32768 = 0.142219
    y = 0x5678; // 22136/32768 = 0.675537
    xtimesy = Fract_XtimesY_Q15(x,y);
    // x*y = 0.142219 * 0.675537     = 0.096069
    // xtimesy = 0x0C4C = 3148/32768 = 0.096069
  </code>

  Remarks:
    Since both x and y are Q15 fixed point numbers the product x*y is also Q15.
*/
fract_q15_t Fract_XtimesY_Q15(fract_q15_t x, fract_q15_t y );


// *****************************************************************************
/* Function:
    fract_q15d16_t Fract_XtimesY_Q15d16(fract_q15d16_t x, fract_q15d16_t y );

  Summary:
    Calculates product of X*Y for Q15.16 fixed point

  Description:
    Calculates product of X*Y for Q15.16 fixed point

  Precondition:
    None

  Parameters:
    x - Q15.16 multiplicand
    y - Q15.16 multiplicand

  Returns:
    Q15.16 product x*y

  Example:

  Remarks:
    Since both x and y are Q15.16 fixed point numbers the product x*y is also Q15.16.
*/
fract_q15d16_t Fract_XtimesY_Q15d16(fract_q15d16_t x, fract_q15d16_t y );


// *****************************************************************************
/* Function:
    fract_q31_t Fract_XtimesY_Q31(fract_q31_t x, fract_q31_t y );


  Summary:
    Calculates product of X*Y for Q31 fixed point

  Description:
    Calculates product of X*Y for Q31 fixed point

  Precondition:
    None

  Parameters:
    x - Q31 multiplicand
    y - Q31 multiplicand

  Returns:
    Q31 product x*y

  Example:

  Remarks:
    Since both x and y are Q31 fixed point numbers the product x*y is also Q31.
*/
fract_q31_t Fract_XtimesY_Q31(fract_q31_t x, fract_q31_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_XtimesY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y )

  Summary:
    Multiply X times Y, for Q15 block floating point arguments.

  Description:
    Multiply X times Y, for Q15 block floating point arguments.

  Precondition:
    None.

  Parameters:
    x - Q15 block floating point multiplicand
    y - Q15 block floating point multiplicand

  Returns:
    Product x*y as Q15 block floating point

  Example:
  <code>
    fract_q15bfp_t X,Y,XtimesY;

    X.m = 0x1234;
    X.e = 0; //  4660/32768 = 0.142219
    Y.m = 0x5678;
    Y.e = 0; // 22136/32768 = 0.675537
    XtimesY = Fract_XtimesY_Q15bfp(X,Y);
    // XtimesY.m = 0x6260 = 25184/32768 = 0.768554
    // XtimesY.e = 0xFFFD = -3
    // XtimesY   = 0.768554>>3   = 0.096069
    // X*Y = 0.142219 * 0.675537 = 0.096069
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_XtimesY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
/* Function:
    fract_q15d16_t Fract_XoverY_Q15d16FromQ15( fract_q15_t x, fract_q15_t y );

  Summary:
    Calculates ratio X/Y for two Q15 fixed points

  Description:
    Calculates ratio X/Y for two Q15 fixed points

  Precondition:
    None.

  Parameters:
    x - Q15 numerator
    y - Q15 denominator

  Returns:
    Ratio x/y as Q15.16 fixed point

  Example:
  <code>
    fract_q15_t    x,y;
    fract_q15d16_t xovery;

    x = 0xDEAD; //  -8531/32768 = -0.260345
    y = 0xBEEF; // -16657/32768 = -0.508331
    xovery = Fract_XoverY_Q15d16FromQ15(x,y);
    // x/y   = -8531/-16657 = 33564/65536 = 0.512157
    // xovery = 0x0000 831C = 33564
  </code>

  Remarks:
    Since ratio x/y can be bigger than unity, result must be Q15.16.
*/
fract_q15d16_t Fract_XoverY_Q15d16FromQ15( fract_q15_t x, fract_q15_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_XoverY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y )

  Summary:
    Division x/y as Q15 block floating point.

  Description:
    Division x/y as Q15 block floating point.

  Precondition:
    None.

  Parameters:
    x - Numerator as Q15 block floating point
    y - Denominator as Q15 block floating point

  Returns:
    Ratio x/y as Q15 block floating point.

  Example:
  <code>
    fract_q15_t x,y;
    fract_q15bfp_t X, Y, XoverY;

    x = 0x1234; //  4660/32768 = 0.142219
    y = 0x5678; // 22136/32768 = 0.675537
    X = Fract_Convert_Q15d16ToQ15bfp((fract_q15d16_t)x);
    Y = Fract_Convert_Q15d16ToQ15bfp((fract_q15d16_t)y);
    XoverY = Fract_XoverY_Q15bfp(X,Y);
    // x/y = 4660/22136 = 6898/32768 = 0.210516
    // XoverY.m = 0x6BC8 = 27592
    // XoverY.e = 0xFFFE = -2
    // XoverY   = (27592/32768)>>2 = 0.210510
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_XoverY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_XoverY_Q15bfpFromQ15(fract_q15_t x, fract_q15_t y )

  Summary:
    Calculate ratio of X/Y as Q15's but return result as Q15 block floating point.

  Description:
    Calculate ratio of X/Y as Q15's but return result as Q15 block floating point.

  Precondition:
    None.

  Parameters:
    x - Q15 numerator
    y - Q15 denominator

  Returns:
    Ratio x/y as Q15 block floating point

  Example:
  <code>
    fract_q15_t x,y;
    fract_q15bfp_t XoverY;

    x = 0x1234; //  4660/32768 = 0.142219
    y = 0x5678; // 22136/32768 = 0.675537
    XoverY = Fract_XoverY_Q15bfpFromQ15(x,y);
    // x/y = 4660/22136 = 6898/32768 = 0.210516
    // XoverY.m = 0x6BC8 = 27592
    // XoverY.e = 0xFFFE = -2
    // XoverY   = (27592/32768)>>2 = 0.210510
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_XoverY_Q15bfpFromQ15(fract_q15_t x, fract_q15_t y );


// *****************************************************************************
// *****************************************************************************
// Section: Q15 fixed point square root
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    fract_q15_t Fract_sqrtQ15(fract_q15_t X)

  Summary:
    Fixed point Q15 squareroot.

  Description:
    Returns square root of Q15 (Q0.15) fraction.  Since -1 <= X < 1, we know that
    the 0<= SquareRoot(||X||) < 1.

  Precondition:
    None.

  Parameters:
    X - Q15 input, should be non-negative.

  Returns:
    Square root of abs(X).

  Example:
  <code>
  </code>

  Remarks:
<pre>
  Given integer X, solve for Y such that

      Y      Y        X
    ---- * ----  =  ----
    2^15   2^15     2^15

  This is equivalent to

     Y * Y = X * 2^15 = X<<15

  So as integers:

     Y = integer sqrt(X<<15)

  thus

    fixed_point_sqrt(X) = integer_sqrt(X<<15)

  For X = 0,1,...32767, define

    ErrSqrt = (sqrt(X/32768.0) - FixedPointSQRT(X))/sqrt(X/32768.0)

  This error ranges from 0 to 0.015259, with an average error of 0.0054096

  The fast integer square root function used is based on
    http://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
  </pre>
*/
fract_q15_t Fract_sqrtQ15(fract_q15_t X);


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Point Logarithms
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    int32_t Fract_log2fix (int32_t X32, size_t precision)

  Summary:
    Calculates fixed point base 2 logarithm using number of fractional bits
    specified by precision argument.

  Description:
    Calculates fixed point base 2 logarithm using number of fractional bits
    specified by precision argument.  This implementation is based on Clay. S.
    Turner's fast binary logarithm algorithm[1].

  Precondition:
    None.

  Parameters:
    X32 - Fixed point input, if x == 0, function returns INT32_MIN to represent
                negative infinity. If x < 0 function returns INT32_MAX as error.
    precision - number of fractional bits in input and output, 1<= precision <= 31.
                if precision < 1 or precision > 31 function returns INT32_MAX as error.

  Returns:
    Logarithm base 2 of input, in same Q format as input.  The Q format used is based on
    the value of precision.  For example, if precision == 16 then the Q format used is
    Q15.16.

  Example:
  <code>
    // Make log output Q15.16
    #define PRECISION 16

    #define STRING_MAX_SIZE 133
    char  ioString[STRING_MAX_SIZE];

    double Xdouble;
    double scale = 1U << PRECISION;
    float  Xfloat;

    int16_t X16;
    int32_t X32;

    while ( Xdouble != -1.0 )
    {
        sprintf(ioString,"\r\nX: ");
        SendDataBuffer(ioString, strlen(ioString) );
        GetDataBuffer(ioString,STRING_MAX_SIZE);
        sscanf(ioString,"%f",&Xfloat);
        Xdouble = Xfloat;

        if ( Xdouble == -1 )
        {
            sprintf(ioString,"\r\nWe're done!\r\n");
            SendDataBuffer(ioString, strlen(ioString) );
            return 0;
        }
        else if ( Xdouble < 0 )
        {
            sprintf(ioString,"%f is Negative!\r\n",Xdouble);
            SendDataBuffer(ioString, strlen(ioString) );
            continue;
        }
        else if ( Xdouble >= 1 << (32 - PRECISION) )
        {
            sprintf(ioString, "%f is too big\r\n", Xdouble);
            SendDataBuffer(ioString, strlen(ioString) );
            continue;
        }

        sprintf(ioString,"   log10(%f) = %f\r\n", Xdouble, (double)log10(Xdouble));
        SendDataBuffer(ioString, strlen(ioString) );

        X32 = Fract_log10fix(Xdouble * scale, PRECISION);
        sprintf(ioString,"Fract_log10fix(%f) = %f (0x%08X)\r\n", Xdouble, X32/scale, X32);
        SendDataBuffer(ioString, strlen(ioString) );

        X32 = Fract_log2fix(Xdouble * scale, PRECISION) ;
        sprintf(ioString," Fract_log2fix(%f) = %f (0x%08X)\r\n", Xdouble, X32/scale, X32 );
        SendDataBuffer(ioString, strlen(ioString) );

    }//end while ( Xdouble != -1.0 )
  </code>

  Remarks:
    If precision = N, then the input and output are Q(31-N).N .
    For example, if precision = 16 then the input and output are Q15.16.
    This means that there are 16 fractional bits (the LS word) and the upper
    16 bits (MS word) represents an int16_t integer.

  References:
    [1] C. S. Turner,  "A Fast Binary Logarithm Algorithm", IEEE Signal
    Processing Mag., pp. 124,140, Sep. 2010.
*/
int32_t Fract_log2fix (int32_t X32, size_t precision);


// *****************************************************************************
/* Function:
    int32_t Fract_log10fix (int32_t x, size_t precision)

    Calculates fixed point base 10 logarithm using number of fractional bits
    specified by precision argument.

  Description:
    Calculates fixed point base 10 logarithm using number of fractional bits
    specified by precision argument.  Log base 10 is calculated using the
    log base 2 function Fract_log2fix.

  Precondition:
    None.

  Parameters:
    x - Fixed point input, if x == 0, function returns INT32_MIN to represent
        negative infinity.  If x is input as a negative number it will be cast
        to be a unsigned 32 bit integer.
    precision - number of fractional bits in input and output, 1<= precision <= 31.
                if precision < 1 or precision > 31 function returns INT32_MAX.

  Returns:
    Logarithm base 10 of input, in same Q format as input.

  Example:
    See the code example under Fract_log2fix.

  Remarks:
    If precision = N, then the input and output are Q(31-N).N .
    For example, if precision = 16 then the input and output are Q15.16.
    This means that there are 16 fractional bits (the LS word) and the upper
    16 bits (MS word) represents an int16_t integer.
*/
int32_t Fract_log10fix(int32_t x, size_t precision);


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Power Functions, pow(2,x), pow(10,x)
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t Fract_Power( uint16_t nBase, uint16_t nExponent )

  Summary:
    Computes pow(nBase,nExponent) using for loop.

  Description:
    Computes pow(nBase,nExponent) using for loop.

  Precondition:
    None

  Parameters:
    nBase     = unsigned integer
    nExponent = unsigned integer

  Returns:
    pow(nBase,nExponent), this must fit into a 32 bit unsigned integer.

  Example:
  <code>
    uint16_t nBase, nExponent;
    uint32_t answer;

    nBase = 5;
    nExponent = 3;
    answer = Fract_Power(nBase,nExponent);
    // Answer = pow(5,3) = 125 = 0x007D
  </code>

  Remarks:
    none
*/
uint32_t Fract_Power( uint16_t nBase, uint16_t nExponent );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_InversePower( uint16_t nBase, uint16_t nExponent )

  Summary:
    Computes pow(nBase,-nExponent).

  Description:
    Computes pow(nBase,-nExponent).

  Precondition:
    None

  Parameters:
    nBase     = unsigned integer
    nExponent = unsigned integer

  Returns:
    pow(nBase,-nExponent), as a Q15bfp block floating point number.

  Example:
  <code>
    uint16_t nBase, nExponent;
    fract_q15bfp_t Answer;

    nBase = 5;
    nExponent = 3;
    Answer = Fract_InversePower(nBase,nExponent);
    // Answer = pow(5,-3) = 1/125
    // Answer.m = 0x4189 = 16777
    // Answer.e = 0xFFFA = -6
    // Answer   = (16777/32768)>>6 = 1/(125.001609)
  </code>

  Remarks:
    pow(nBase,nExponent) must fit inside a uint32_t variable.
*/
fract_q15bfp_t Fract_InversePower( uint16_t nBase, uint16_t nExponent );


// *****************************************************************************
/* Function:
    fract_q15_t Fract_2toMinusX_Q15( fract_q15_t x );

  Summary:
    Calculates pow(2,-x) with fixed point math.

  Description:
    Calculates pow(2,-x) with fixed point math, using three term Taylor Series
    and 256 entry lookup table.

  Precondition:
    None.

  Parameters:
    x - Exponent in calculating 2^-x.  If x < 0 then -x is used.

  Returns:
    Value of pow(2,-x)

  Example:
  <code>
    fract_q15_t x,y;

    x = 3277; // 0.1
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.1) = 0.933033 = 30574 / 32768
    // y = 0x776D = 30573

    x = 6554; // 0.2
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.2) = 0.870551 = 28526 / 32768
    // y = 0x6F6E = 28526

    x = 9830; // 0.3
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.3) = 0.812252 = 26616 / 32768
    // y = 0x67F9 = 26617

    x = 13107; // 0.4
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.4) = 0.757858 = 24834 / 32768
    // y = 0x6102 = 24834

    x = 16384; // 0.5
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.5) = 0.707107 = 23170 / 32768
    // y = 0x5A81 = 23169

    x = 3277 + 16384; // 0.1 + 0.5 = 0.6
    y = Fract_2toMinusX_Q15(x);
    // pow(2,0.6) = 0.659754 = 21619 / 32768
    // y = 0x5472 = 21618

    x = 6554 + 16384; // 0.2 + 0.5 = 0.7
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.7) = 0.615572 = 20171 / 32768
    // y = 0x4ECA = 20170

    x = 9830 + 16384; // 0.3 + 0.5 = 0.8
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.8) = 0.574349 = 18820 / 32768
    // y = 0x4984 = 18820

    x = 13107 + 16384; // 0.4 + 0.5 = 0.9
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-0.9) = 0.535887 = 17560 / 32768
    // y = 0x4498 = 17560

    x = 32767; // 0.5 + 0.5 = 1.0
    y = Fract_2toMinusX_Q15(x);
    // pow(2,-1.0) = 0.5 = 16384 / 32768
    // y = 0x4001 = 16385
  </code>

  Remarks:
    If 0 <= x < 1, then pow(2,-x) is in the range 0.5 < pow(2,-x) <= 1.
*/
fract_q15_t Fract_2toMinusX_Q15( fract_q15_t x );


// *****************************************************************************
/* Function:
    fract_q15d16_t Fract_2toX_Q15d16( fract_q15d16_t x )

  Summary:
    Calculates pow(2,x) with fixed point math.

  Description:
    Calculates pow(2,x) using fixed point math, with Q15.16 x, returning Q15.16 result

  Precondition:
    None.

  Parameters:
    x - exponent, in Q15.16 format

  Returns:
    pow(2,x), in Q15.16 format

  Example:
  <code>
    fract_q15d16_t x,y;

    x = 0x00008000; // 0.5 in Q15.16
    y = Fract_2toX_Q15d16(x);
    // pow(2,0.5) = sqrt(2) = 1.414213 = 92682/65536 = 0x0001 6A0A
    // y = 0x0001 6A14 = 92692/65536 = 1.414367 (vs. 1.414213)

    x = 0x00014000; // 1.25 in Q15.16
    y = Fract_2toX_Q15d16(x);
    // pow(2,1.25) = 2.378414 = 155,872 / 65536 = 0x0002 60E0
    // y = 0x0002 60E0
  </code>

  Remarks:
    None.
*/
fract_q15d16_t Fract_2toX_Q15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_2toX_Q15bfpFromQ15d16( fract_q15d16_t x )

  Summary:
    Calculates pow(2,x) with fixed point math.

  Description:
    Calculates pow(2,x) using fixed point math, based on input in Q15.16 format,
    returning result in Q15 block floating point format.

  Precondition:
    None.

  Parameters:
    x - exponent, in Q15.16 format

  Returns:
    pow(2,x), in Q15 block floating point format

  Example:
  <code>
    fract_q15d16_t x;
    fract_q15bfp_t Y;

    x = 0x00008000; // 0.5 in Q15.16
    Y = Fract_2toX_Q15bfpFromQ15d16(x);
    // pow(2,0.5) = sqrt(2) = 1.414213 = 46341 / 32768 = 0x5A82<<1
    // Y.m = 0x5A85 = 23173
    // Y.e = 1
    // Y = ( 23173 / 32768 )<<1 = 1.414367 (vs. 1.414213)

    x = 0x00014000; // 1.25 in Q15.16
    Y = Fract_2toX_Q15bfpFromQ15d16(x);
    // pow(2,1.25) = 2.378414 = 77936 / 32768 = 0x4C1C<<2
    // Y.m = 0x4C1 = 19484
    // Y.e = 2
    // Y = ( 19484/32768 ) << 2 = 2.378418 (vs. 2.378414)
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_2toX_Q15bfpFromQ15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_2toX_Q15bfp( fract_q15bfp_t x )

  Summary:
    Calculates pow(2,x) with fixed point math.

  Description:
    Calculates pow(2,x) using fixed point math, from Q15 block floating point x,
    returning Q15 block floating point result.

  Precondition:
    none.

  Parameters:
    x - exponent, in Q15 block floating point format

  Returns:
    pow(2,x), in Q15 block floating point format

  Example:
  <code>
    fract_q15bfp_t x, y;

    x.m = 16384; // 0.5
    x.e = 0;
    y = Fract_2toX_Q15bfp(x);
    // pow(2,0.5) = sqrt(2) = 46341/32768;
    // y.m = 0x5A85 = 23173
    // y.e = 1
    // y = (23173/32768)<<1 = 1.414368 (vs. 1.414214)

    x.m = 24576; // 0.75
    x.e = 0;
    y = Fract_2toX_Q15bfp(x);
    // pow(2,0.75) = 1.681793 = 55109/32768;
    // y.m = 0x6BA4 = 27556
    // y.e = 1
    // y = (27556/32768)<<1 = 1.681885 (vs. 1.681793)
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t Fract_2toX_Q15bfp( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q15d16_t Fract_10toX_Q15d16( fract_q15d16_t x )

  Summary:
    Calculates pow(2,x) with fixed point math.

  Description:
    Calculates pow(2,x) using fixed point math, with x as Q15.16, returning result
    as Q15.16.

  Precondition:
    None.

  Parameters:
    x - exponent as Q15.16

  Returns:
    pow(2,x) as Q15.16

  Example:
  <code>
     fract_q15d16_t x,y;

     x = 0x00008000; // 0.5
     y = Fract_10toX_Q15d16(x);
     // y = 0x0003 2990 = 207,248/65536 = 3.162353 (vs. 3.162277) , rel error = 1/41688

     x = 0x0003243F; // 205,887 / 65536 = 3.141586 (vs. 3.14159)
     y = Fract_10toX_Q15d16(x);
     // y = 0x0569 6000 = 90,791,936/65536 = 1385.375 (vs. 1385.435475) , rel error = 1/22909
  </code>

  Remarks:
    Fract_10toX(x) is basically Fract_2toX( log2(10)*x ).
*/
fract_q15d16_t Fract_10toX_Q15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_10toX_Q15bfp( fract_q15bfp_t x )

  Summary:
    Calculates pow(2,x) with fixed point math.

  Description:
    Calculates pow(2,x) using fixed point math, with x as Q15 block floating point,
    returning the result in the same format.

  Precondition:
    None.

  Parameters:
    x - exponent, in Q15 block floating point format

  Returns:
    pow(2,x), in Q15 block floating point format

  Example:
  <code>
    fract_q15bfp_t X,Y;

    X.m = 0x4000; // 1/2
    X.e = 0;
    Y = Fract_10toX_Q15bfp(X);
    // Y.m = 0x6532 = 25906
    // Y.e = 2
    // Y = ( 25906 / 32768 )<<2 = 3.162353 (vs. 3.162277) , rel error = 1/41688

    X.m = 0x6488; // (3.14159/4)
    X.e = 2;
    Y = Fract_10toX_Q15bfp(X);
    // Y.m = 0x5697 = 22167 / 32768 = 0.676483
    // Y.e = 0x000B = 11
    // Y = (22167 / 32768)<<11 = 1385.4375 (vs. 1385.435475), rel error = 1/684,023
  </code>

  Remarks:
    Fract_10toX(x) is basically Fract_2toX( log2(10)*x ).
*/
fract_q15bfp_t Fract_10toX_Q15bfp( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t Fract_10toX_Q15bfpFromQ15d16( fract_q15d16_t x )

  Summary:
    Calculates pow(2,x) with fixed point math.

  Description:
    Calculates pow(2,x) using fixed point math, with x as Q15.16, returning
    result as Q15 block floating point.

  Precondition:
    None.

  Parameters:
    x - exponent as Q15.16

  Returns:
    pow(2,x) as Q15 block floating point

  Example:
  <code>
    fract_q15bfp_t Y;

    x = 0x00008000; // 0.5
    Y = Fract_10toX_Q15bfpFromQ15d16(x);
    // Y.m = 0x6532 = 25906
    // Y.e = 2
    // Y = ( 25906 / 32768 )<<2 = 3.162353 (vs. 3.162277) , rel error = 1/41688

    x = 0x0003243F; // 205,887 / 65536 = 3.141586 (vs. 3.14159)
    Y = Fract_10toX_Q15bfpFromQ15d16(x);
    // Y.m = 0x5696 = 22167 / 32768 = 0.676483
    // Y.e = 0x000B = 11
    // Y = (22167 / 32768)<<11 = 1385.4375 (vs. 1385.435475), rel error = 1/684,023
  </code>

  Remarks:
    Fract_10toX(x) is basically Fract_2toX( log2(10)*x ).
*/
fract_q15bfp_t Fract_10toX_Q15bfpFromQ15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15_t LUTwInterpolation_Q15( fract_q15_t x,
                                       const fract_q15_t * pX_LUT_Vector,
                                       const fract_q15_t * pY_LUT_Vector,
                                       const uint16_t nLUTSize );

  Summary:
    Calculates calculates y = f(x) based on a lookup table with linear
    interpolation between table entries.

  Description:
    Calculates calculates y = f(x) based on a lookup table with linear
    interpolation between table entries.

  Precondition:
    None.

  Parameters:
    x             - Q15 value, independent variable
    pX_LUT_Vector - X column of LUT in Q15
    pY_LUT_Vector - Y column of LUT in Q15
    nLutSize      - number of rows (entries) in the LUT

  Returns:
    y =  f(x) based on the LUT using linear interpolation between table entries.

  Example:

  Remarks:
    It is assumed that X LUT vector is in strictly monotonically increasing order.
    That is: pX_LUT_Vector[i] < pX_LUT_Vector[i+1] for i = 0,1,...nLUTSize-2

    Internal math is done in signed 32 integers and truncated to Q15.

    If x <= pX_LUT_Vector[0] then pY_LUT_Vector[0] is returned.

    if pX_LUT_Vector[nLUTSize-1] <= x then pY_LUT_Vector[nLUTSize-1] is returned.

  WARNING:
    It is strongly recommended that you test out the LUT and compare actual vs.
    desired values in Matlab/Octave or Excel!  You may need to add additional
    rows where the interpolation does not produce the needed accuracy.
*/
fract_q15_t LUTwInterpolation_Q15( fract_q15_t x,
                                   const fract_q15_t * pX_LUT_Vector,
                                   const fract_q15_t * pY_LUT_Vector,
                                   const uint16_t nLUTSize );


//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif  /* _LIBQ_C_H_ */
