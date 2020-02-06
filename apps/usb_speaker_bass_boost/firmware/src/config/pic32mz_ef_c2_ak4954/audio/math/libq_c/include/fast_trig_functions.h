/*******************************************************************************
  Fast Trig Functions using the LibQC Library

  Company:
    Microchip Technology Inc.

  File Name:
     fast_trib_functions.h

  Summary:
    Fast Trig Functions using the LibQC Library

  Description:
    sin and cosine of angle sin degrees using a Lookup Table (LUT) and fixed
    point (integer) arithmetic.

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

#ifndef _FAST_TRIG_FUNCS_H_      // Guards against multiple inclusion
#define _FAST_TRIG_FUNCS_H_

#include "audio/math/libq_c/libq_c.h"

// LUT for sin(degrees), 0 <= degrees <= 90
#include "audio/math/libq_c/include/XInDegrees_LUT_91.h"
#include "audio/math/libq_c/include/SinX_LUT_91.h"

// *****************************************************************************
/* Function:
    fract_q0d15_t SinInDegs( fract_q9d6_t theta );

  Summary:
    Calculates fixed point Sin(theta) (Q0.15 fixed point),
    where theta is in degrees (Q8.6 fixed point)

  Description:
    Calculates fixed point Sin(theta) (Q0.15 fixed point),
    where theta is in degrees (Q8.6 fixed point)

  Precondition:
    None

  Parameters:
    theta - Q9.6 fixed point angle

  Returns:
    sin(theta) in Q0.15 fixed point

  Example:
  <code>
    int32_t nTests = 400;
    float theta;
    float delta_theta = 360.0/(nTests-1);
    float theta0 = 0;

    int32_t i;
    int32_t x;

    for ( i = 0; i < nTests; i++ )
    {
        theta = theta0 + i*delta_theta;
        x = theta*(32768/512); //Q9.6
        printf("%d, %d \r\n",(int)x,SinInDegs(x)));
    }
  </code>

  Remarks:
    None.
*/
fract_q0d15_t SinInDegs( fract_q9d6_t theta );


// *****************************************************************************
/* Function:
    fract_q0d15_t CosInDegs( fract_q9d6_t theta );

  Summary:
    Calculates fixed point Cos(theta) (Q0.15 fixed point),
    where theta is in degrees (Q8.6 fixed point)

  Description:
    Calculates fixed point Cos(theta) (Q0.15 fixed point),
    where theta is in degrees (Q8.6 fixed point)

  Precondition:
    None

  Parameters:
    theta - Q9.6 fixed point angle

  Returns:
    cos(theta) in Q0.15 fixed point

  Example:
  <code>
    int32_t nTests = 400;
    float theta;
    float delta_theta = 360.0/(nTests-1);
    float theta0 = 0;

    int32_t i;
    int32_t x;

    for ( i = 0; i < nTests; i++ )
    {
        theta = theta0 + i*delta_theta;
        x = theta*(32768/512); //Q9.6
        printf("%d, %d \r\n",(int)x,CosInDegs(x));
    }
  </code>

  Remarks:
    CosInDegs uses SinInDegs.
*/
fract_q0d15_t CosInDegs( fract_q9d6_t theta );

#endif //#ifndef _FAST_TRIG_FUNCS_H_
