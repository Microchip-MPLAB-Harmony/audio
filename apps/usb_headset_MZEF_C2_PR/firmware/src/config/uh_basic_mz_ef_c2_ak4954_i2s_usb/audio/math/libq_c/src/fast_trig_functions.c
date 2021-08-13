/*******************************************************************************
  PIC32 Fast Trig Functions

  Company:
    Microchip Technology Inc.

  File Name:
     fast_trig_functions.c

  Summary:
     Provides sin and cosine in degrees using 16-bit fixed point math and
     a Look Up Table for sin(theta), 0<= theta <= 90 degrees.

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
//DOM-IGNORE-END

#include "audio/math/libq_c/include/fast_trig_functions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Sin(x), x in degrees
// *****************************************************************************
// *****************************************************************************

// Angles in Q9.6 fixed point.
#define QD9D6_090 ( 90<<6)
#define QD9D6_180 (180<<6)
#define QD9D6_270 (270<<6)
#define QD9D6_360 (360<<6)

fract_q0d15_t SinInDegs( fract_q9d6_t theta )
{
    fract_q0d15_t sin_x;

    // Normalize angle to be in [0,360)
    while ( theta < 0 )
    {
        theta += QD9D6_360;
    }

    while ( QD9D6_360 <= theta )
    {
        theta -= QD9D6_360;
    }

    if ( theta <= QD9D6_090 )
    {
        sin_x = LUTwInterpolation_Q15(theta,_1stQuadrantX_LUT,_1stQuadrantSin_LUT,(uint16_t)FIRST_QUADRANT_SIN_LUT_LENGTH);
    }
    else if ( theta <= QD9D6_180 )
    {
         sin_x = LUTwInterpolation_Q15(QD9D6_180-theta,_1stQuadrantX_LUT,_1stQuadrantSin_LUT,(uint16_t)FIRST_QUADRANT_SIN_LUT_LENGTH);
    }
    else if ( theta <= QD9D6_270 )
    {
        sin_x = -LUTwInterpolation_Q15(theta-QD9D6_180,_1stQuadrantX_LUT,_1stQuadrantSin_LUT,(uint16_t)FIRST_QUADRANT_SIN_LUT_LENGTH);
    }
    else // theta < 360 degrees
    {
        sin_x = -LUTwInterpolation_Q15(QD9D6_360-theta,_1stQuadrantX_LUT,_1stQuadrantSin_LUT,(uint16_t)FIRST_QUADRANT_SIN_LUT_LENGTH);
    }

    return sin_x;
}


fract_q0d15_t CosInDegs( fract_q9d6_t theta )
{
    return -SinInDegs(theta - QD9D6_090);
}
