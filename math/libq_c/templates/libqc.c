/*******************************************************************************
  PIC32 Fixed Point in C (LibQC) Library

  Company:
    Microchip Technology Inc.

  File Name:
     libqc.h

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
         Q0.15  (Q15)          16         libqc_q0d15_t
new      Q3.12  (Q15)          16         libqc_q3d12_t (radian angles)
new      Q9.6   (Q15)          16         libqc_q9d6_t  (degree angles)
new      Q10.5  (Q15)          16         libqc_q10d6_t (pixel coordinates, -1024 <= x < 1023)
         Q0.31  (Q31)          32         libqc_q0d31_t
         Q15.16 (Q31)          32         libqc_q15d16_t
         Q16.15 (Q31)          32         libqc_q16d15_t
         Q0.63  (Q64)          64         libqc_q0d63_t

    The following block floating point types are supported:
      Mantissa/Exponent
         15     16             32         libqc_q15bfp_t
         31     32             64         libqc_q31bfp_t

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

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <assert.h>
#include <stdio.h>
#include "../libqc.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables
// *****************************************************************************
// *****************************************************************************
//static libqc_q31_t  _2toMinusX_LUT[] = //1x257 lookup table
#include "./libqc/LUT__3Terms__nLUT_256.h"

// *****************************************************************************
// *****************************************************************************
// Section: Fixed Point Type Conversions
// *****************************************************************************
// *****************************************************************************

libqc_q15d16_t LIBQC_Convert_Q15bfpToQ15d16( libqc_q15bfp_t x )
{
    libqc_q15d16_t xQ15d16 = ((libqc_q15d16_t)x.m)<<1; // Q15 to Q16;
    if ( x.e < 0 )
    {
        return xQ15d16>>(-x.e);
    }
    else if ( x.e > 0 )
    {
        return xQ15d16<<x.e;
    }
    else
    {
        return xQ15d16;
    }

}//LIBQC_Convert_Q15bfpToQ15d16


libqc_q31_t LIBQC_Convert_Q15bfpToQ31( libqc_q15bfp_t x )
{
    libqc_q31_t ans;
    libqc_q15bfp_t xtemp;

    if     ( x.e == 0 )
    {
        ans = x.m<<16;  // Shift from Q15 to Q31
    }
    else if( x.e < 0 )
    {
        ans = x.m<<16;  // Shift from Q15 to Q31
        ans = ans>>(-x.e); // Add exponent
    }
    else
    { // Exponent > 0, This conversion may not work.
        xtemp = LIBQC_Convert_Q15ToQ15bfp(x.m);
        if ( x.e - xtemp.e <= 0 )
        { // Leading bits in x.m allow number to fit into Q31
            ans = x.m<<16;  // Shift from Q15 to Q31
            ans = ans<<x.m; // Add exponent without losing bits
        }
        else
        { // (x.m)<<x.e won't fit into Q31
            if ( x.m < 0 )
            {
                ans = INT32_MIN;
            }
            else
            {
                ans = INT32_MAX;
            }
        }

        ans = ans<<x.e;
    }

    return ans;

}//LIBQC_Convert_Q15bfpToQ31


libqc_q15bfp_t LIBQC_Convert_Q15ToQ15bfp( libqc_q15_t x )
{
    bool           neg_x = false;
    libqc_q15bfp_t xbfp;

    xbfp.e = 0;
    if ( x == 0 )
    {
        xbfp.m = 0;
    }
    else // x != 0
    { // Mantissa ratio already in Q15 range
        if ( x < 0 )
        {
            neg_x = true;
            x = -x;
        }

        // Remove leading zeros
        while( x < 1<<((16-1)-1) )
        {
            x = x<<1;
            xbfp.e--;
        }

        if ( neg_x )
        { // Apply sign
            xbfp.m = -x;
        }
        else
        {
            xbfp.m = x;
        }

    }//end if ( x == 0 )

    return xbfp;

}//LIBQC_Convert_Q15ToQ15bfp


libqc_q15bfp_t LIBQC_Convert_Q15d16ToQ15bfp( libqc_q15d16_t x )
{
    bool           neg_x = false;
    int16_t        x_int;
    libqc_q15bfp_t xbfp;

    xbfp.e = 0;
    if ( x == 0 )
    {
        xbfp.m = 0;
    }
    else // x != 0
    {
        if ( x < 0 )
        {
            neg_x = true;
            x = -x;
        }
        x_int = x>>16;

        // Rescale to remove leading zeros
        if ( x_int == 0 )
        { // Mantissa ratio already in Q15 range
            x = x>>1; // Q15.16 to Q15.
            // Remove leading zeros
            while( x < 1<<((16-1)-1) )
            {
                x = x<<1;
                xbfp.e--;
            }
        }
        else
        { // Mantissa >=  one
            // Normalize mantissa to be less than unity
            while( x >= 1<<15 )
            {
                x = x>>1;
                xbfp.e++;
            }
            xbfp.e--; //Q16 to Q15
        }//end if ( x_int == 0 )

        if ( neg_x )
        { // Apply sign
            xbfp.m = -x;
        }
        else
        {
            xbfp.m = x;
        }

    }//end if ( x == 0 )

    return xbfp;

}//LIBQC_Convert_Q15d16ToQ15bfp


// *****************************************************************************
// *****************************************************************************
// Section: Q15 Block Floating Point Add/Subtract Operations
// *****************************************************************************
// *****************************************************************************

libqc_q15bfp_t LIBQC_XplusY_Q15bfp( libqc_q15bfp_t x, libqc_q15bfp_t y )
{
    libqc_q15bfp_t  x_plus_y;
    int32_t         xm_plus_ym;
    int16_t         delta_e;

    if      ( x.e > y.e )
    {
        x_plus_y.e = x.e;
        delta_e = x.e-y.e;
        delta_e = delta_e > 16 ? 16 : delta_e;
        xm_plus_ym = (x.m<<delta_e) + y.m;
        xm_plus_ym = xm_plus_ym>>delta_e;
    }
    else if ( x.e < y.e )
    {
        x_plus_y.e = y.e;
        delta_e = y.e-x.e;
        delta_e = delta_e > 16 ? 16 : delta_e;
        xm_plus_ym = x.m + (y.m<<delta_e);
        xm_plus_ym = xm_plus_ym>>delta_e;
    }
    else
    {
        x_plus_y.e = x.e;
        xm_plus_ym = x.m + y.m;
    }

    if( xm_plus_ym < -32768 || 32767 < xm_plus_ym )
    {
        xm_plus_ym = xm_plus_ym>>1;
        x_plus_y.e++;
    }
    x_plus_y.m = xm_plus_ym;

    // Note:
    //  Removing leading bits in mantissa isn't necessary.
    //  It doesn't increase accuracy of the results.
    return x_plus_y;

}//LIBQC_XplusY_Q15bfp


libqc_q15bfp_t LIBQC_XminusY_Q15bfp( libqc_q15bfp_t x, libqc_q15bfp_t y )
{
    libqc_q15bfp_t  x_minus_y;
    int32_t         xm_minus_ym;
    int16_t         delta_e;

    if      ( x.e > y.e )
    {
        x_minus_y.e = x.e;
        delta_e = x.e-y.e;
        delta_e = delta_e > 16 ? 16 : delta_e;
        xm_minus_ym = (((int32_t)x.m)<<delta_e) - y.m;
        xm_minus_ym = xm_minus_ym>>delta_e;
    }
    else if ( x.e < y.e )
    {
        x_minus_y.e = y.e;
        delta_e = y.e-x.e;
        delta_e = delta_e > 16 ? 16 : delta_e;
        xm_minus_ym = x.m - ((int32_t)(y.m)<<delta_e);
        xm_minus_ym = xm_minus_ym>>delta_e;
    }
    else
    {
        x_minus_y.e = x.e;
        xm_minus_ym = x.m - y.m;
    }

    if( xm_minus_ym < -32768 || 32767 < xm_minus_ym )
    {
        xm_minus_ym = xm_minus_ym>>1;
        x_minus_y.e++;
    }
    x_minus_y.m = xm_minus_ym;

    // Note:
    //  Removing leading bits in mantissa isn't necessary.
    //  It doesn't increase accuracy of the results.
    return x_minus_y;

}//LIBQC_XminusY_Q15bfp


// *****************************************************************************
// *****************************************************************************
// Section: Multiply and Divide Operations
// *****************************************************************************
// *****************************************************************************

libqc_q15_t LIBQC_XtimesY_Q15(libqc_q15_t x, libqc_q15_t y )
{
    int32_t xy;

    xy = ((int32_t)x) * ((int32_t)y);
    return (libqc_q15_t)(xy>>15);

}//LIBQC_XtimesY_Q15


libqc_q31_t LIBQC_XtimesY_Q31(libqc_q31_t x, libqc_q31_t y )
{
    int64_t xy;

    xy = ((int64_t)x) * ((int64_t)y);
    return (libqc_q15d16_t)(xy>>31);

}//LIBQC_XtimesY_Q15


libqc_q15d16_t LIBQC_XtimesY_Q15D16(libqc_q15d16_t x, libqc_q15d16_t y )
{
    int64_t xy;

    xy = ((int64_t)x) * ((int64_t)y);
    return (libqc_q15d16_t)(xy>>16);

}//LIBQC_XtimesY_Q15


libqc_q15bfp_t LIBQC_XtimesY_Q15bfp(libqc_q15bfp_t x, libqc_q15bfp_t y )
{
    bool           neg_product = false;
    int32_t        mx_times_my;
    libqc_q15bfp_t xy;

    xy.e = x.e + y.e;

    mx_times_my = ((int32_t)x.m) * ((int32_t)y.m);
    if ( mx_times_my == 0 )
    {
        xy.m = 0;
        xy.e = 0;
    }
    else
    {
        if ( mx_times_my < 0 )
        {
            neg_product = true;
            mx_times_my = - mx_times_my;
        }

        // Normalize mantissa to be greater than 1/2 but less than 1
        while( mx_times_my < 1<<(31-1-1) )
        { // remove leading zero bit
            mx_times_my = mx_times_my<<1;
            xy.e--;
        }
        xy.m = mx_times_my>>15;
        if ( neg_product )
        { // Apply sign
            xy.m = -xy.m;
        }
    }//end if ( mx_times_my == 0 )

    return xy;

} //LIBQC_XtimesY_Q15bfp


libqc_q15d16_t LIBQC_XoverY_Q15d16FromQ15( libqc_q15_t x, libqc_q15_t y )
{
    int32_t x_over_y;

    x_over_y = ( ((int32_t)x)<<16 ) / y;
    return x_over_y;

}//LIBQC_XoverY_Q15d16FromQ15


libqc_q15bfp_t LIBQC_XoverY_Q15bfp(libqc_q15bfp_t x, libqc_q15bfp_t y )
{
    libqc_q15bfp_t x_over_y;
    libqc_q15d16_t mx_over_my;
    libqc_q15bfp_t xm_over_my_bfp;

    x_over_y.e = x.e - y.e;

    mx_over_my = LIBQC_XoverY_Q15d16FromQ15(x.m,y.m);

    xm_over_my_bfp = LIBQC_Convert_Q15d16ToQ15bfp(mx_over_my);

    x_over_y.m = xm_over_my_bfp.m;
    x_over_y.e += xm_over_my_bfp.e;

    return x_over_y;

}//LIBQC_XoverY_Q15bfp


libqc_q15bfp_t LIBQC_XoverY_Q15bfpFromQ15(libqc_q15_t x, libqc_q15_t y )
{
    libqc_q15bfp_t xbfp,ybfp;

    xbfp = LIBQC_Convert_Q15d16ToQ15bfp((libqc_q15d16_t)x);
    ybfp = LIBQC_Convert_Q15d16ToQ15bfp((libqc_q15d16_t)y);
    return LIBQC_XoverY_Q15bfp(xbfp,ybfp);

}//LIBQC_XoverY_Q15bfpFromQ15


// *****************************************************************************
// *****************************************************************************
// Section: Q15 fixed point square root
// *****************************************************************************
// *****************************************************************************

/* Fast square root function taken from
  http://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
 */
static int32_t _ftbl[33] =
    {0,1,1,2,2,4,5,8,11,16,22,32,45,64,90,128,181,256,362,512,724,1024,1448,
    2048,2896,4096,5792,8192,11585,16384,23170,32768,46340};
static int32_t _ftbl2[32] =
    {32768,33276,33776,34269,34755,35235,35708,36174,36635,37090,37540,37984,
    38423,38858,39287,39712,40132,40548,40960,41367,41771,42170,42566,42959,
    43347,43733,44115,44493,44869,45241,45611,45977};

static int16_t _integer_sqrt(int32_t val)
{
    int16_t cnt=0;
    int32_t t = val;
    while (t)
    {
        cnt++;
        t>>=1;
    }
    if (6>=cnt)
    {
        t=(val<<(6-cnt));
    }
    else
    {
        t=(val>>(cnt-6));
    }

    return (_ftbl[cnt]*_ftbl2[t&31])>>15;

}//_integer_sqrt


int16_t LIBQC_sqrtQ15(libqc_q15_t X)
{
    int16_t absX;
    int32_t X32;

    absX = X < 0 ? -X : X;
    X32 = ((int32_t)absX)<<15;
    return _integer_sqrt(X32);

}//LIBQC_sqrtQ15


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Point Logarithms
// *****************************************************************************
// *****************************************************************************
int32_t LIBQC_log2fix (int32_t X32, size_t precision)
{
    size_t iBit; // index to current fractional bit
    int32_t bit = 1U << (precision - 1); // current fractional bit value
    int32_t Y32 = 0;
    uint32_t x;

    if (precision < 1 || precision > 31 || X32 < 0) {
        return INT32_MAX; // indicates an error
    }
    x = X32;

    if (x == 0) {
        return INT32_MIN; // represents negative infinity
    }

    while (x < 1U << precision) {
        x <<= 1;
        Y32 -= 1U << precision;
    }

    while (x >= 2U << precision) {
        x >>= 1;
        Y32 += 1U << precision;
    }

    uint64_t z = x;

    // iBit = 0 corresponds to MSB of LS word (fractional part)
    for (iBit = 0; iBit < precision; iBit++) {
        z = z * z >> precision;
        if (z >= 2U << precision) {
            z >>= 1;
            Y32 += bit;
        }
        bit >>= 1;
    }

    return Y32;

}//LIBQC_log2fix


int32_t LIBQC_log10fix (int32_t x, size_t precision)
{
    int64_t t;
    int32_t log2x;
    int32_t yout;

    x = x < 0 ? -x : x; // Make sure x >= 0

    log2x = LIBQC_log2fix(x, precision);
    if     ( log2x == INT32_MIN )
    {
        yout = INT32_MIN;
    }
    else if ( log2x == INT32_MAX )
    {
        yout = INT32_MAX;
    }
    else
    {
        t =  log2x * INV_LOG2_10_Q1DOT31;
        if ( t > 0 )
        {
            yout =  t >> 31;
        }
        else
        {
            yout =  -(-t>>31);
        }
    }

    return yout;

}//LIBQC_log10fix


// *****************************************************************************
// *****************************************************************************
// Section: Fixed Power Functions, pow(2,x), pow(10,x)
// *****************************************************************************
// *****************************************************************************

uint32_t LIBQC_Power( uint16_t nBase, uint16_t nExponent )
{
    uint16_t i;
    uint32_t product = 1;

    for ( i = 1; i <= nExponent; i++ )
    {
        product *= nBase;
    }
    return product;

}//LIBQC_Power


libqc_q15bfp_t LIBQC_InversePower( uint16_t nBase, uint16_t nExponent )
{
    libqc_q15bfp_t returnValue;
    uint32_t   x;
    uint32_t   m; // mantissa
    int16_t  exp; // power of 2 exponent

    m = LIBQC_Power(nBase, nExponent);
    x = m;
    exp = 0;
    while( x > 2 && exp <= 15 )
    {
        x = x>>1;
        exp++;
    }
    returnValue.m = (1ul<<(exp+15))/m; // 1/m = pow(2,exp)/x as Q15
    returnValue.e = -exp;

    while ( returnValue.m < 16384 )
    { // Remove leading zeros
        returnValue.m = returnValue.m<<1;
        returnValue.e--;
    }

    return returnValue;

}//LIBQC_InversePower


#define Q31_ONE     0x7FFFFFFF   // pow(2,31 - 1
#define LOGTWO      22713        // ln(2)     = 0.693146 = 22713/32768
#define ROOT_TWO_OVER_TWO 23170  // sqrt(2)/2 = 0.707107 = 23170/32768
libqc_q15_t LIBQC_2toMinusX_Q15( libqc_q15_t x )
{
    bool bAddFactor = false;
    uint16_t    iLUT;
    libqc_q15_t epsilon;
    libqc_q15_t logTwoEpsilon;
    libqc_q31_t temp32;

    if ( x < 16384 )
    {// Less than 1/2
        epsilon = x;
    }
    else
    {
        epsilon = x - 16384;
        bAddFactor = true;
    }
    logTwoEpsilon = LIBQC_XtimesY_Q15(LOGTWO,epsilon);

    // Index for LUT
    iLUT = (N_2toMinusX_LUT*((libqc_q31_t)epsilon))/16384;
    iLUT = iLUT > N_2toMinusX_LUT ? N_2toMinusX_LUT : iLUT;

    // Calculate pow(2,-x using three term Taylor series and LUT
    temp32 = logTwoEpsilon*( (logTwoEpsilon>>1) - 32768 );
    temp32 = Q31_ONE + (temp32<<1) - _2toMinusX_LUT[iLUT];
    if ( bAddFactor )
    {
        temp32 = ROOT_TWO_OVER_TWO*(temp32>>15);
    }

    return (libqc_q15_t)(temp32>>16);

}//LIBQC_2toMinusX


libqc_q15d16_t LIBQC_2toX_Q15d16( libqc_q15d16_t x )
{
    libqc_q15bfp_t xbfp;

    xbfp = LIBQC_2toX_Q15bfpFromQ15d16(x);
    return LIBQC_Convert_Q15bfpToQ15d16( xbfp );

}//LIBQC_2toX_Q15d16


libqc_q15bfp_t LIBQC_2toX_Q15bfp( libqc_q15bfp_t x )
{
    libqc_q15d16_t xQ15d16;
    libqc_q15bfp_t ans;

    xQ15d16 = LIBQC_Convert_Q15bfpToQ15d16(x);
    ans = LIBQC_2toX_Q15bfpFromQ15d16( xQ15d16 );

    return ans;

}//LIBQC_2toX_Q15bfp


libqc_q15bfp_t LIBQC_2toX_Q15bfpFromQ15d16( libqc_q15d16_t x )
{
    bool xNegative;
    int16_t k;
    libqc_q0d16_t fracX;
    libqc_q15_t epsilon;
    libqc_q15bfp_t ans;

    if ( x >= 0 )
    {
        xNegative = false;
        k = (int16_t)(x>>16);
        fracX = ((libqc_q0d16_t)( 0x0000FFFF & x ))>>1;
        epsilon = 32767 - fracX; // 1 - fracX as fixed points
    }
    else
    {
        xNegative = true;
        x = -x;
        k = (int16_t)(x>>16);
        fracX = ((libqc_q0d16_t)( 0x0000FFFF & x ))>>1;
        epsilon = fracX;
    }
    ans.m = LIBQC_2toMinusX_Q15(epsilon);
    ans.e = xNegative ? -k : 1+k ;

    return ans;
}
//LIBQC_2toX_Q15bfpFromQ15d16


static libqc_q15d16_t _Log10Base2_Q15d16 = 0x0003526A;

libqc_q15d16_t LIBQC_10toX_Qd15d16( libqc_q15d16_t x )
{
    int64_t temp64;
    libqc_q15d16_t log10Base2_times_x;
    libqc_q15d16_t y;

    temp64 = ((int64_t)_Log10Base2_Q15d16)*x;
    log10Base2_times_x = temp64>>16;
    y = LIBQC_2toX_Q15d16(log10Base2_times_x);

    return y;

}//LIBQC_10toX


libqc_q15bfp_t LIBQC_10toX_Q15bfp( libqc_q15bfp_t xbfp )
{
    libqc_q15d16_t x,y;
    libqc_q15bfp_t ybfp;

    x = LIBQC_Convert_Q15bfpToQ15d16(xbfp);
    y = LIBQC_10toX_Qd15d16(x);
    ybfp = LIBQC_Convert_Q15d16ToQ15bfp(y);

    return ybfp;

}//LIBQC_10toX_Q15bfp


libqc_q15bfp_t LIBQC_10toX_Q15bfpFromQ15d16( libqc_q15d16_t x )
{
    libqc_q15d16_t y;
    libqc_q15bfp_t ybfp;

    y = LIBQC_10toX_Qd15d16(x);
    ybfp = LIBQC_Convert_Q15d16ToQ15bfp(y);

    return ybfp;

}//LIBQC_10toX_Q15bfp


// *****************************************************************************
// *****************************************************************************
// Section: Table Lookup with Interpolation
// *****************************************************************************
// *****************************************************************************
//static bool _bIsBefore(libqc_q15_t x, int iLut, const libqc_q15_t * pX_LUT_Vector, const uint16_t nLUTSize)
//{
//    if( iLut < 0 || nLUTSize - 1 < iLut )
//    {
//        return false;
//    }
//    return x < pX_LUT_Vector[iLut];
//}

static bool _bIsAfter(libqc_q15_t x, int iLut, const libqc_q15_t * pX_LUT_Vector, const uint16_t nLUTSize)
{
    if( iLut < 0 || nLUTSize - 1 < iLut )
    {
        return false;
    }
    return pX_LUT_Vector[iLut+1] <= x;
}

static bool _bIsInside(libqc_q15_t x, int iLut, const libqc_q15_t * pX_LUT_Vector, const uint16_t nLUTSize)
{
    if( iLut < 0 || nLUTSize - 1 < iLut )
    {
        return false;
    }
    return ( pX_LUT_Vector[iLut] <= x && x < pX_LUT_Vector[iLut+1] );
}

libqc_q15_t LUTwInterpolation_Q15( libqc_q15_t x,
                                   const libqc_q15_t * pX_LUT_Vector,
                                   const libqc_q15_t * pY_LUT_Vector,
                                   const uint16_t nLUTSize )
{
    int iLUT, iFirst, iLast, iMiddle;
    libqc_q15_t deltaX, deltaY, y;
    libqc_q15d16_t slope;

    // Note:  It is assumed that X LUT vector is in strictly monotonically increasing order.

    if ( x <= pX_LUT_Vector[0] )
    { // x is before or at start of the table
        return pY_LUT_Vector[0]; // return first element from y
    }

    if ( pX_LUT_Vector[nLUTSize-1] <= x )
    { // x is after or at end of the table
        return pY_LUT_Vector[nLUTSize-1]; // return last element from y
    }

//  //Old way of finding iLUT
//    for ( iLUT = 0; iLUT < nLUTSize-1; iLUT++ )
//    {
//        if ( pX_LUT_Vector[iLUT] <= x && x < pX_LUT_Vector[iLUT+1] )
//        {
//            break;
//        }
//    }
//    iFound = iLUT;

  //New way using binary search for iLUT
    iLUT    = 0; // Fail safe in case binary search fails...
    iFirst  = 0;
    iLast   = nLUTSize - 1;
    iMiddle = ( iFirst + iLast )/2;
    while ( iFirst <= iLast )
    {
        if( _bIsAfter(x,iMiddle,pX_LUT_Vector,nLUTSize))
        {
            iFirst = iMiddle + 1;
        }
        else if ( _bIsInside(x,iMiddle,pX_LUT_Vector,nLUTSize) )
        {
            iLUT = iMiddle;
            break;
        }
        else
        {
            iLast = iMiddle - 1;
        }
        iMiddle = ( iFirst + iLast )/2;
    }

    deltaX = pX_LUT_Vector[iLUT+1] - pX_LUT_Vector[iLUT];
    deltaY = pY_LUT_Vector[iLUT+1] - pY_LUT_Vector[iLUT];

    // NOTE: slope limited to -32768 <= slope < 32768
    slope  = LIBQC_XoverY_Q15d16FromQ15(deltaY,deltaX);

    deltaX = x - pX_LUT_Vector[iLUT];
    y = pY_LUT_Vector[iLUT] + ((slope*deltaX)>>16);

    return  y;

}//LUTwInterpolation_Q15
