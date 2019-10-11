/*******************************************************************************
  PIC32MX/MZ Fixed Point in C (LibQC) Library

  Company:
    Microchip Technology Inc.

  File Name:
     libqc_fixedpoint.h

  Summary:
    PIC32MX/MZ fixed point typedefs for LibQC Library.

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
new      Q10.5  (Q15)          16         libqc_q10d5_t (pixel coordinates, -1024 <= x < 1023)
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
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _LIBQC_FIXEDPOINT_H_      // Guards against multiple inclusion
#define _LIBQC_FIXEDPOINT_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <inttypes.h>

// *****************************************************************************
// *****************************************************************************
// Section: Fixed Data Types
// *****************************************************************************
// *****************************************************************************
// // *****************************************************************************

/* Fixed point data types

  Summary:
    Fixed point data types used within Libq_c.

  Description:
    This section defines the fixed point data types used with the
    libq_c fixed point library.

  Remarks:
    The underlying data type for most fixed point types is a signed integer.
    However, multiple fixed point types are defined so that the real
    value can be inferred from an internal integer value.
*/

// Fractional data types. 
typedef int16_t q15; //q15 n.n (signed)

typedef int32_t q31; //q31 n.n (signed)

typedef int64_t q63; //Q63 n.n (signed)

typedef int16_t i16; //Q16d0

// FxQFloat32 pseudo floating point type (limited floating point)
// NOTE:  Extended FxQflExt32 used with f2Qfloat32) 
struct 
{
  q15   man;
  i16   exp;
} FxQFloat32;

// *****************************************************************************
/* Fixed Point Integer Typedef for Q0.15

  Summary:
    Typedef for the Q0.15 fixed point data type.

  Description:
    Typedef for the Q0.15 fixed point data type into a 16 bit signed integer.
    Values for this data type are in the range [-1,+1), i.e. -1 <= x < +1.

  Example:
  <code>
    float Xfloat;
    libqc_q0d15_t Xq0d15;

    Xfloat = Xq0d15/32768.0;
  </code>

  Remarks:
  This data type has one sign bit and 15 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    Sfffffffffffffff
 </c></pre>
*/
typedef int16_t        libqc_q0d15_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q15

  Summary:
    Typedef for the Q15 fixed point data type.

  Description:
    Typedef for the Q15 fixed point data type into a 16 bit signed integer.
    Values for this data type are in the range [-1,+1), i.e. -1 <= x < +1.
    Note this is functionally identical to libqc_q0d15_t.

  Example:
  <code>
    float Xfloat;
    libqc_q15_t Xq15;

    Xfloat = Xq15/32768.0;
  </code>

  Remarks:
  This data type has one sign bit and 15 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    Sfffffffffffffff
 </c></pre>
*/
typedef libqc_q0d15_t   libqc_q15_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q0.16

  Summary:
    Typedef for the Q0.16 fixed point data type.

  Description:
    Typedef for the Q0.16 fixed point data type into a 16 bit unsigned integer.
    Values for this data type are in the range [0,+1], i.e. 0 <= x <= +1.

  Example:
  <code>
    float Xfloat;
    libqc_q0d16_t Xq0d16;

    Xfloat = Xq0d16/65536.0;
  </code>

  Remarks:
  This data type has no sign bit and 16 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    ffffffffffffffff
 </c></pre>
*/
typedef uint16_t       libqc_q0d16_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q16

  Summary:
    Typedef for the Q16 fixed point data type.

  Description:
    Typedef for the Q16 fixed point data type into a 16 bit unsigned integer.
    Values for this data type are in the range [0,+1], i.e. 0 <= x <= +1.
    Note this is functionally identical to libqc_q0d16_t.

  Example:
  <code>
    float Xfloat;
    libqc_q16_t Xq16;

    Xfloat = Xq16/65536.0;
  </code>

  Remarks:
  This data type has no sign bit and 16 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    ffffffffffffffff
 </c></pre>
*/
typedef uint16_t       libqc_q16_t;

// *****************************************************************************
/* Fixed Point Integer Typedef for Q3.12

  Summary:
    Typedef for the . fixed point data type.

  Description:
    Typedef for the Q3.12 fixed point data type into a 16 bit signed integer.
    Values for this data type are in the range [-8,+8), i.e. -8 <= x < +8.
    This is larger than -2*pi <= x <= 2*pi, so it is ideal for radian angles.

  Example:
  <code>
    float Xfloat;
    libqc_q0d15_t Xq3d12;

    Xfloat = Xq3d12/4096.0;
    Xq3d12 = (libqc_q0d15_t)(Xfloat*4096);
  </code>

  Remarks:
  This data type has one sign bit, 3 integer bits, and 12 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    Siiiffffffffffff
 </c></pre>
*/
typedef int16_t        libqc_q3d13_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q9.6

  Summary:
    Typedef for the . fixed point data type.

  Description:
    Typedef for the Q9.6 fixed point data type into a 16 bit signed integer.
    Values for this data type are in the range [-512,+512), i.e. -512 <= x < +512.
    This is larger than -360 <= x <= 360, so it is ideal for degree angles.

  Example:
  <code>
    float Xfloat;
    libqc_q9d6_t Xq9d6;

    Xfloat = Xq9d6/64.0;
    Xq9d6 = (libqc_q9d6_t)(Xfloat*64);
  </code>

  Remarks:
  This data type has one sign bit, 9 integer bits, and 6 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    Siiiiiiiiiffffff
 </c></pre>
*/
typedef int16_t        libqc_q9d6_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q10.5

  Summary:
    Typedef for the Q10.5 fixed point data type.

  Description:
    Typedef for the Q10.5 fixed point data type into a 16 bit signed integer.
    Values for this data type are in the range [-1024,+2014), i.e. -1024 <= x < +1024.
    Thus, this data type will support pixel coordinates for screens larger than 1000 pixels.

  Example:
  <code>
    float Xfloat;
    libqc_q10d5_t Xq10d5;

    Xfloat = Xq10d5/32.0;
    Xq10d5 = (libqc_q10d5_t)(Xfloat*32);
  </code>

  Remarks:
  This data type has one sign bit, 10 integer bits, and 5 fractional bits in a 16 bit word.
<pre><c>
    -----1----------
    5432109876543210
    ----____----____
    Siiiiiiiiiifffff
 </c></pre>
*/
typedef int16_t        libqc_q10d5_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q0.31

  Summary:
    Typedef for the Q0.31 fixed point data type.

  Description:
    Typedef for the Q0.15 fixed point data type into a 32 bit signed integer.
    Values for this data type are in the range [-1,+1), i.e. -1 <= x < +1.

  Example:
  <code>
    float Xfloat;
    libqc_q0d31_t Xq0d31;

    Xfloat = Xq0d31/((float)2<<31);
  </code>

  Remarks:
  This data type has one sign bit and 31 fractional bits in a 32 bit word.
<pre><c>
    -3---------2---------1----------
    10987654321098765432109876543210
    ----____----____----____----____
    Sfffffffffffffffffffffffffffffff
</c></pre>
*/
typedef int32_t        libqc_q0d31_t;


// *****************************************************************************
/* Fixed Point Integer Typedef for Q31

  Summary:
    Typedef for the Q0.31 fixed point data type.

  Description:
    Typedef for the Q0.15 fixed point data type into a 32 bit signed integer.
    Values for this data type are in the range [-1,+1), i.e. -1 <= x < +1.
    Note: this typedef is functionally equivalent to libqc_q0d31_t.

  Example:
  <code>
    float Xfloat;
    libqc_q31_t Xq31;

    Xfloat = Xq31/((float)1<<31);
  </code>

  Remarks:
  This data type has one sign bit and 31 fractional bits in a 32 bit word.
<pre><c>
    -3---------2---------1----------
    10987654321098765432109876543210
    ----____----____----____----____
    Sfffffffffffffffffffffffffffffff
</c></pre>
*/
typedef libqc_q0d31_t   libqc_q31_t;


/**************************************************************************
  Summary:
    Typedef for the Q15.16 fixed point data type

  Description:
    Fixed Point Integer Typedef

    Typedef for the Q15.16 fixed point data type into 32 bit signed integer
    Values for this data type are in the range [-32768,+32768), i.e. -32768
    \<= x \< +32768.
  Example:
    <code>
      float Xfloat;
      libqc_q15d16_t Xq15d16;

      Xfloat = Xq15d16/65536.0;
    </code>
  Remarks:
    This data type has one sign bit, 15 integer bits, and 16 fractional
    bits.
    <pre>

    <c>    -3---------2---------1----------</c>
    <c>    10987654321098765432109876543210</c>
    <c>    ----____----____----____----____</c>
    <c>    Siiiiiiiiiiiiiiiffffffffffffffff</c>

    </pre>

  **************************************************************************/
typedef int32_t        libqc_q15d16_t;


/**********************************************************************
  Summary:
    Typedef for the Q16d15 fixed point data type
  Description:
    Fixed Point Integer Typedef

    Typedef for the Q16d15 fixed point data type into a 32 bit signed
    integer Values for this data type are in the range [-65536,+65536),
    i.e. -65536 \<= x \< +65536.
  Example:
    <code>
      float Xfloat;
      libqc_q16d15_t Xq16d15;

      Xfloat = Xq16d15/32768.0;
    </code>
  Remarks:
    This data type has one sign bit, 16 integer bits, and 15 fractional
    bits.
    <pre>

    <c>    -3---------2---------1----------</c>
    <c>    10987654321098765432109876543210</c>
    <c>    ----____----____----____----____</c>
    <c>    Siiiiiiiiiiiiiiiifffffffffffffff</c>

    </pre>

  **********************************************************************/
typedef int32_t        libqc_q16d15_t;


// *****************************************************************************
/* Fixed Point Integer Typedef

  Summary:
    Typedef for the Q0.63 fixed point data type

  Description:
    Typedef for the Q0.63 fixed point data type into a 64 bit signed integer.
    Values for this data type are in the range [-1,+1), i.e. -1 <= x < +1.

  Remarks:
    This data type has one sign bit and 63 fractional bits.
<pre><c>
    ---6---------5---------4---------3---------2---------1----------
    3210987654321098765432109876543210987654321098765432109876543210
    Sfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
</c></pre>
*/
typedef int64_t        libqc_q0d63_t;


/******************************************************************************
  Summary:
    Typedef for the Q0.63 fixed point data type
  Description:
    Fixed Point Integer Typedef

    Typedef for the Q0.63 fixed point data type into a 64 bit signed
    integer. Values for this data type are in the range [-1,+1), i.e. -1 \<=
    x \< +1.
  Remarks:
    This data type has one sign bit and 63 fractional bits.
<pre><c>
    ---6---------5---------4---------3---------2---------1----------
    3210987654321098765432109876543210987654321098765432109876543210
    Sfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
</c></pre>

  ******************************************************************************/
typedef libqc_q0d63_t   libqc_q63_t;


// *****************************************************************************
// *****************************************************************************
// Section: Block Floating Point Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Q15 Block Floating Point Typedef

  Summary:
    Fixed point data structure that emulates floating point, with Q15 mantissa.

  Description:
    Fixed point data structure used to emulate real IEEE floating point using
    Q15 mantissa.

    Note, none of the special tricks implied in real floating point apply here.
    For example, the mantissa could have leading bits that could be removed
    by adjusting the exponent.  Or, the exponent could be zero.

  Remarks:
    Example:
  <code>
    libqc_q15bfp_t x; // Original number
    libq_q0d15_t   y; // equivalent fixed point.
    if ( x.e < 0 )
    { // Negative exponent ==> right shift mantissa
        if ( x.m < 0 )
        { // Right shifting negative numbers can be dangerous!
          // (Right shifting a negative number is implementation defined.)
          // This way enforces extending the sign bit:
            y = -((-x.m)>>x.e);
        }
        else
        {
            y = (x.m)>>x.e;
        }
    }
    else
    { // Positive Exponent ==> left shift mantissa (Overflow possible)
        y = (x.m)<<x.e;
    }
  </code>
*/
typedef struct
{
    libqc_q15_t m; // Mantissa
    int16_t     e; // Exponent

} libqc_q15bfp_t;


// *****************************************************************************
/* Q31 Block Floating Point Typedef

  Summary:
    Fixed point data structure that emulates floating point, with Q31 mantissa.

  Description:
    Fixed point data structure used to emulate real IEEE floating point using
    Q31 mantissa.

    Note, none of the special tricks implied in real floating point apply here.
    For example, the mantissa could have leading bits that could be removed
    by adjusting the exponent.  Or, the exponent could be zero.

  Remarks:
    Example:
  <code>
    libqc_q31bfp_t x; // Original number
    libq_q0d31_t   y; // equivalent fixed point.
    if ( x.e < 0 )
    {
        if ( x.m < 0 )
        { // Right shifting negative numbers can be dangerous!
          // (Right shifting a negative number is implementation defined.)
          // This way enforces extending the sign bit:
            y = -((-x.m)>>x.e);
        }
        else
        {
            y = (x.m)>>x.e;
        }
    else
    { // Positive Exponent ==> left shift mantissa (Overflow possible)
        y = (x.m)<<x.e;
    }
  </code>
    In a more compact form:
  <code>
    y = x.e < 0 ? (x.m)>>x.e : (x.m)<<x.e;
  </code>
*/
typedef struct
{
    libqc_q31_t m; // Mantissa
    int32_t     e;  // Exponent

} libqc_q31bfp_t;


#endif  /* _LIBQC_FIXEDPOINT_H_ */

