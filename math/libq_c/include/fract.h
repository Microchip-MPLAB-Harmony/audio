/*******************************************************************************
  PIC32 Fixed Point in C (LibQC) Library

  Company:
    Microchip Technology Inc.

  File Name:
     fract.h

  Summary:
    PIC32 fixed point typedefs and primitives in C.

  Description:
    Signed fixed point types are defined as follows:

        Qn.m where:
            - n is the number of data bits to the left of the radix point
            - m is the number of data bits to the right of the radix point
            - a signed bit is implied

    Unique variable types for fractional representation are also defined:

        Exact Name     # Bits Required    Type
         Q0.15  (Q15)          16         fract_q0d15_t
new      Q3.12  (Q15)          16         fract_q3d12_t (radian angles)
new      Q9.6   (Q15)          16         fract_q9d6_t  (degree angles)
new      Q10.5  (Q15)          16         fract_q10d5_t (pixel coordinates, -1024 <= x < 1023)
         Q0.31  (Q31)          32         fract_q0d31_t
         Q15.16 (Q31)          32         fract_q15d16_t
         Q16.15 (Q31)          32         fract_q16d15_t
         Q0.63  (Q64)          64         fract_q0d63_t

    The following block floating point types are supported:
      Mantissa/Exponent
         15     16             32         fract_q15bfp_t
         31     32             64         fract_q31bfp_t

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

#ifndef _LIBQC_H_      // Guards against multiple inclusion
#define _LIBQC_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
#define INV_LOG2_10_Q1DOT31 UINT64_C(0x268826a1) // Inverse log base 2 of 10
#define LOG10_2 0x00004D10L // = log10(2), To convert dB into dBFS add this to output of LIBQC_log10fix


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
#include "audio/math/include/fract_fixedpoint.h"


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Point Type Conversions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    fract_q15d16_t LIBQC_Convert_Q15bfpToQ15d16( fract_q15bfp_t x )

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

    x_Q15d16 = LIBQC_Convert_Q15bfpToQ15d16( x_Q15bfp );
    // x_Q15d16 = 0x0000 4000
  </code>

  Remarks:
    No effort is made to trap overflows of underflows,
*/
fract_q15d16_t LIBQC_Convert_Q15bfpToQ15d16( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q31_t LIBQC_Convert_Q15bfpToQ31( fract_q15bfp_t x )

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
fract_q31_t LIBQC_Convert_Q15bfpToQ31( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_Convert_Q15ToQ15bfp( fract_q15_t x )

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
    x_Q15bfp = LIBQC_Convert_Q15d16ToQ15bfp(x_Q15d16);
    //x_Q15bfp.m = 0xC000  // -1/2
    //x_Q15bfp.e = 0xFFFF = -1; // -1/2>>1 = -1/4
  </code>

  Remarks:
    Mantissa and exponent are adjusted to remove redundant leading bits.
*/
fract_q15bfp_t LIBQC_Convert_Q15ToQ15bfp( fract_q15_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_Convert_Q15d16ToQ15bfp( fract_q15d16_t x )

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
    x_Q15bfp = LIBQC_Convert_Q15d16ToQ15bfp(x_Q15d16);
    //x_Q15bfp.m = 0xC000  // -1/2
    //x_Q15bfp.e = 0xFFFF = -1; // -1/2>>1 = -1/4
  </code>

  Remarks:
    Mantissa and exponent are adjusted to remove redundant leading bits.
*/
fract_q15bfp_t LIBQC_Convert_Q15d16ToQ15bfp( fract_q15d16_t x );


// *****************************************************************************
// *****************************************************************************
// Section: Q15 Block Floating Point Add/Subtract Operations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_XplusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y )

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
    X = LIBQC_Convert_Q15d16ToQ15bfp(x);
    Y = LIBQC_Convert_Q15d16ToQ15bfp(y);
    XplusY = LIBQC_XplusY_Q15bfp( X, Y );
    // XplusY.m = 0x5000 = 20480/32768 = 0.625
    // XPlusY.e = 4
    // XPlusY = 0.625<<4 = 10
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_XplusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_XminusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y )

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
    X = LIBQC_Convert_Q15d16ToQ15bfp(x);
    Y = LIBQC_Convert_Q15d16ToQ15bfp(y);
    XminusY = LIBQC_XminusY_Q15bfp( X, Y );
    // XplusY.m = 0xC000 = -16384/32768 = -0.5
    // XPlusY.e = 3
    // XPlusY = -0.5<<3 = -4
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_XminusY_Q15bfp( fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
// *****************************************************************************
// Section: Multiply and Divide Operations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    fract_q15_t LIBQC_XtimesY_Q15(fract_q15_t x, fract_q15_t y );

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
    xtimesy = LIBQC_XtimesY_Q15(x,y);
    // x*y = 0.142219 * 0.675537     = 0.096069
    // xtimesy = 0x0C4C = 3148/32768 = 0.096069
  </code>

  Remarks:
    Since both x and y are Q15 fixed point numbers the product x*y is also Q15.
*/
fract_q15_t LIBQC_XtimesY_Q15(fract_q15_t x, fract_q15_t y );


// *****************************************************************************
/* Function:
    fract_q15d16_t LIBQC_XtimesY_Q15d16(fract_q15d16_t x, fract_q15d16_t y );

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
fract_q15d16_t LIBQC_XtimesY_Q15d16(fract_q15d16_t x, fract_q15d16_t y );


// *****************************************************************************
/* Function:
    fract_q31_t LIBQC_XtimesY_Q31(fract_q31_t x, fract_q31_t y );


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
fract_q31_t LIBQC_XtimesY_Q31(fract_q31_t x, fract_q31_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_XtimesY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y )

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
    XtimesY = LIBQC_XtimesY_Q15bfp(X,Y);
    // XtimesY.m = 0x6260 = 25184/32768 = 0.768554
    // XtimesY.e = 0xFFFD = -3
    // XtimesY   = 0.768554>>3   = 0.096069
    // X*Y = 0.142219 * 0.675537 = 0.096069
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_XtimesY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
/* Function:
    fract_q15d16_t LIBQC_XoverY_Q15d16FromQ15( fract_q15_t x, fract_q15_t y );

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
    xovery = LIBQC_XoverY_Q15d16FromQ15(x,y);
    // x/y   = -8531/-16657 = 33564/65536 = 0.512157
    // xovery = 0x0000 831C = 33564
  </code>

  Remarks:
    Since ratio x/y can be bigger than unity, result must be Q15.16.
*/
fract_q15d16_t LIBQC_XoverY_Q15d16FromQ15( fract_q15_t x, fract_q15_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_XoverY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y )

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
    X = LIBQC_Convert_Q15d16ToQ15bfp((fract_q15d16_t)x);
    Y = LIBQC_Convert_Q15d16ToQ15bfp((fract_q15d16_t)y);
    XoverY = LIBQC_XoverY_Q15bfp(X,Y);
    // x/y = 4660/22136 = 6898/32768 = 0.210516
    // XoverY.m = 0x6BC8 = 27592
    // XoverY.e = 0xFFFE = -2
    // XoverY   = (27592/32768)>>2 = 0.210510
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_XoverY_Q15bfp(fract_q15bfp_t x, fract_q15bfp_t y );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_XoverY_Q15bfpFromQ15(fract_q15_t x, fract_q15_t y )

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
    XoverY = LIBQC_XoverY_Q15bfpFromQ15(x,y);
    // x/y = 4660/22136 = 6898/32768 = 0.210516
    // XoverY.m = 0x6BC8 = 27592
    // XoverY.e = 0xFFFE = -2
    // XoverY   = (27592/32768)>>2 = 0.210510
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_XoverY_Q15bfpFromQ15(fract_q15_t x, fract_q15_t y );


// *****************************************************************************
// *****************************************************************************
// Section: Q15 fixed point square root
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    fract_q15_t LIBQC_sqrtQ15(fract_q15_t X)

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
fract_q15_t LIBQC_sqrtQ15(fract_q15_t X);


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Point Logarithms
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    int32_t LIBQC_log2fix (int32_t X32, size_t precision)

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

        X32 = LIBQC_log10fix(Xdouble * scale, PRECISION);
        sprintf(ioString,"LIBQC_log10fix(%f) = %f (0x%08X)\r\n", Xdouble, X32/scale, X32);
        SendDataBuffer(ioString, strlen(ioString) );

        X32 = LIBQC_log2fix(Xdouble * scale, PRECISION) ;
        sprintf(ioString," LIBQC_log2fix(%f) = %f (0x%08X)\r\n", Xdouble, X32/scale, X32 );
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
int32_t LIBQC_log2fix (int32_t X32, size_t precision);


// *****************************************************************************
/* Function:
    int32_t LIBQC_log10fix (int32_t x, size_t precision)

    Calculates fixed point base 10 logarithm using number of fractional bits
    specified by precision argument.

  Description:
    Calculates fixed point base 10 logarithm using number of fractional bits
    specified by precision argument.  Log base 10 is calculated using the
    log base 2 function LIBQC_log2fix.

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
    See the code example under LIBQC_log2fix.

  Remarks:
    If precision = N, then the input and output are Q(31-N).N .
    For example, if precision = 16 then the input and output are Q15.16.
    This means that there are 16 fractional bits (the LS word) and the upper
    16 bits (MS word) represents an int16_t integer.
*/
int32_t LIBQC_log10fix(int32_t x, size_t precision);


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Power Functions, pow(2,x), pow(10,x)
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t LIBQC_Power( uint16_t nBase, uint16_t nExponent )

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
    answer = LIBQC_Power(nBase,nExponent);
    // Answer = pow(5,3) = 125 = 0x007D
  </code>

  Remarks:
    none
*/
uint32_t LIBQC_Power( uint16_t nBase, uint16_t nExponent );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_InversePower( uint16_t nBase, uint16_t nExponent )

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
    Answer = LIBQC_InversePower(nBase,nExponent);
    // Answer = pow(5,-3) = 1/125
    // Answer.m = 0x4189 = 16777
    // Answer.e = 0xFFFA = -6
    // Answer   = (16777/32768)>>6 = 1/(125.001609)
  </code>

  Remarks:
    pow(nBase,nExponent) must fit inside a uint32_t variable.
*/
fract_q15bfp_t LIBQC_InversePower( uint16_t nBase, uint16_t nExponent );


// *****************************************************************************
/* Function:
    fract_q15_t LIBQC_2toMinusX_Q15( fract_q15_t x );

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
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.1) = 0.933033 = 30574 / 32768
    // y = 0x776D = 30573

    x = 6554; // 0.2
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.2) = 0.870551 = 28526 / 32768
    // y = 0x6F6E = 28526

    x = 9830; // 0.3
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.3) = 0.812252 = 26616 / 32768
    // y = 0x67F9 = 26617

    x = 13107; // 0.4
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.4) = 0.757858 = 24834 / 32768
    // y = 0x6102 = 24834

    x = 16384; // 0.5
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.5) = 0.707107 = 23170 / 32768
    // y = 0x5A81 = 23169

    x = 3277 + 16384; // 0.1 + 0.5 = 0.6
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,0.6) = 0.659754 = 21619 / 32768
    // y = 0x5472 = 21618

    x = 6554 + 16384; // 0.2 + 0.5 = 0.7
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.7) = 0.615572 = 20171 / 32768
    // y = 0x4ECA = 20170

    x = 9830 + 16384; // 0.3 + 0.5 = 0.8
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.8) = 0.574349 = 18820 / 32768
    // y = 0x4984 = 18820

    x = 13107 + 16384; // 0.4 + 0.5 = 0.9
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-0.9) = 0.535887 = 17560 / 32768
    // y = 0x4498 = 17560

    x = 32767; // 0.5 + 0.5 = 1.0
    y = LIBQC_2toMinusX_Q15(x);
    // pow(2,-1.0) = 0.5 = 16384 / 32768
    // y = 0x4001 = 16385
  </code>

  Remarks:
    If 0 <= x < 1, then pow(2,-x) is in the range 0.5 < pow(2,-x) <= 1.
*/
fract_q15_t LIBQC_2toMinusX_Q15( fract_q15_t x );


// *****************************************************************************
/* Function:
    fract_q15d16_t LIBQC_2toX_Q15d16( fract_q15d16_t x )

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
    y = LIBQC_2toX_Q15d16(x);
    // pow(2,0.5) = sqrt(2) = 1.414213 = 92682/65536 = 0x0001 6A0A
    // y = 0x0001 6A14 = 92692/65536 = 1.414367 (vs. 1.414213)

    x = 0x00014000; // 1.25 in Q15.16
    y = LIBQC_2toX_Q15d16(x);
    // pow(2,1.25) = 2.378414 = 155,872 / 65536 = 0x0002 60E0
    // y = 0x0002 60E0
  </code>

  Remarks:
    None.
*/
fract_q15d16_t LIBQC_2toX_Q15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_2toX_Q15bfpFromQ15d16( fract_q15d16_t x )

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
    Y = LIBQC_2toX_Q15bfpFromQ15d16(x);
    // pow(2,0.5) = sqrt(2) = 1.414213 = 46341 / 32768 = 0x5A82<<1
    // Y.m = 0x5A85 = 23173
    // Y.e = 1
    // Y = ( 23173 / 32768 )<<1 = 1.414367 (vs. 1.414213)

    x = 0x00014000; // 1.25 in Q15.16
    Y = LIBQC_2toX_Q15bfpFromQ15d16(x);
    // pow(2,1.25) = 2.378414 = 77936 / 32768 = 0x4C1C<<2
    // Y.m = 0x4C1 = 19484
    // Y.e = 2
    // Y = ( 19484/32768 ) << 2 = 2.378418 (vs. 2.378414)
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_2toX_Q15bfpFromQ15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_2toX_Q15bfp( fract_q15bfp_t x )

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
    y = LIBQC_2toX_Q15bfp(x);
    // pow(2,0.5) = sqrt(2) = 46341/32768;
    // y.m = 0x5A85 = 23173
    // y.e = 1
    // y = (23173/32768)<<1 = 1.414368 (vs. 1.414214)

    x.m = 24576; // 0.75
    x.e = 0;
    y = LIBQC_2toX_Q15bfp(x);
    // pow(2,0.75) = 1.681793 = 55109/32768;
    // y.m = 0x6BA4 = 27556
    // y.e = 1
    // y = (27556/32768)<<1 = 1.681885 (vs. 1.681793)
  </code>

  Remarks:
    None.
*/
fract_q15bfp_t LIBQC_2toX_Q15bfp( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q15d16_t LIBQC_10toX_Qd15d16( fract_q15d16_t x )

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
     y = LIBQC_10toX_Qd15d16(x);
     // y = 0x0003 2990 = 207,248/65536 = 3.162353 (vs. 3.162277) , rel error = 1/41688

     x = 0x0003243F; // 205,887 / 65536 = 3.141586 (vs. 3.14159)
     y = LIBQC_10toX_Qd15d16(x);
     // y = 0x0569 6000 = 90,791,936/65536 = 1385.375 (vs. 1385.435475) , rel error = 1/22909
  </code>

  Remarks:
    LIBQC_10toX(x) is basically LIBQC_2toX( log2(10)*x ).
*/
fract_q15d16_t LIBQC_10toX_Qd15d16( fract_q15d16_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_10toX_Q15bfp( fract_q15bfp_t x )

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
    Y = LIBQC_10toX_Q15bfp(X);
    // Y.m = 0x6532 = 25906
    // Y.e = 2
    // Y = ( 25906 / 32768 )<<2 = 3.162353 (vs. 3.162277) , rel error = 1/41688

    X.m = 0x6488; // (3.14159/4)
    X.e = 2;
    Y = LIBQC_10toX_Q15bfp(X);
    // Y.m = 0x5697 = 22167 / 32768 = 0.676483
    // Y.e = 0x000B = 11
    // Y = (22167 / 32768)<<11 = 1385.4375 (vs. 1385.435475), rel error = 1/684,023
  </code>

  Remarks:
    LIBQC_10toX(x) is basically LIBQC_2toX( log2(10)*x ).
*/
fract_q15bfp_t LIBQC_10toX_Q15bfp( fract_q15bfp_t x );


// *****************************************************************************
/* Function:
    fract_q15bfp_t LIBQC_10toX_Q15bfpFromQ15d16( fract_q15d16_t x )

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
    Y = LIBQC_10toX_Q15bfpFromQ15d16(x);
    // Y.m = 0x6532 = 25906
    // Y.e = 2
    // Y = ( 25906 / 32768 )<<2 = 3.162353 (vs. 3.162277) , rel error = 1/41688

    x = 0x0003243F; // 205,887 / 65536 = 3.141586 (vs. 3.14159)
    Y = LIBQC_10toX_Q15bfpFromQ15d16(x);
    // Y.m = 0x5696 = 22167 / 32768 = 0.676483
    // Y.e = 0x000B = 11
    // Y = (22167 / 32768)<<11 = 1385.4375 (vs. 1385.435475), rel error = 1/684,023
  </code>

  Remarks:
    LIBQC_10toX(x) is basically LIBQC_2toX( log2(10)*x ).
*/
fract_q15bfp_t LIBQC_10toX_Q15bfpFromQ15d16( fract_q15d16_t x );


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

#endif  /* _LIBQC_H_ */
