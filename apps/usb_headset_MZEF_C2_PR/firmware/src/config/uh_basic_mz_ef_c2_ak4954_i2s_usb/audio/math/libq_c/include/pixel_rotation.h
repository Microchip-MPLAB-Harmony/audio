/*******************************************************************************
  Pixel Rotation Primitives using Fixed Point

  Company:
    Microchip Technology Inc.

  File Name:
     pixel_rotation.h

  Summary:
    Provides rotation transformation of 2-d pixel coordinates based on
    center of rotation and rotation angle in degrees.

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

#ifndef _PIXEL_ROTATION_H_      // Guards against multiple inclusion
#define _PIXEL_ROTATION_H_

#include "audio/math/libq_c/libq_c.h"
#include "audio/math/libq_c/include/fast_trig_functions.h"

// LUT for sin(degrees), 0 <= degrees <= 90
#include "audio/math/libq_c/include/XInDegrees_LUT_91.h"
#include "audio/math/libq_c/include/SinX_LUT_91.h"

#if !defined(GFX_COMMON_H)
// *****************************************************************************
/* Structure:
    GFX_Point_t

  Summary:
    A two dimensional Cartesian point.
*/
typedef struct GFX_Point_t
{
    int32_t x;
    int32_t y;
} GFX_Point;
#endif//!defined(GFX_COMMON_H)

// *****************************************************************************
/* Function:
    GFX_Point RotatePixel( fract_q9d6_t theta,
                           GFX_Point* pCenter,
                           GFX_Point* pSourcePoint );

  Summary:
    Rotates source point arount rotation center by angle defined by theta in degress.

  Description:
    Rotates source point arount rotation center by angle defined by theta in degress.

  Precondition:
    None

  Parameters:
    theta - Q9.6 fixed point angle
    pCenter - pointer to rotation center
    pSourcePoint - pointer to point to be rotated

  Returns:
    Pixel coordinates of rotatation of source point around center defined by rotation center.

  Example:
  <code>
    fract_q10d5_t xSource, ySource;
    fract_q10d5_t xDest, yDest;
    fract_q10d5_t xCenter, yCenter;
    GFX_Point ptSource, ptDest, ptCenter;
    fract_q9d6_t thetaRHC; // Theta in right hand coordinate system;
    fract_q9d6_t thetaLHC; // Theta in left hand coordinate system of display;
    fract_q9d6_t theta0 = 0;
    fract_q9d6_t deltaTheta;
    int iTheta;
    int nTheta = 361;

    deltaTheta = (fract_q9d6_t)( (360.0/(nTheta-1))*(1<<6) );

  //Xcenter = (400,300) on SVGA display
    xCenter = 400<<5; //Scale to Q10.5
    yCenter = 300<<5;
    ptCenter.x = xCenter;
    ptCenter.y = yCenter;

  //Xsource = (600,300) on SVGA Display
    xSource = 600<<5; //Scale to Q10.5
    ySource = 300<<5;
    ptSource.x = xSource;
    ptSource.y = ySource;

    printf("Source-->Destination\r\n");
    for (iTheta = 0; iTheta < nTheta; iTheta++)
    {
        thetaRHC = theta0 + iTheta*deltaTheta;
        thetaLHC = -thetaRHC;
        ptDest = RotatePixel(thetaLHC,&ptCenter,&ptSource);
        printf("%d, %d, %ld,%ld, %ld,%ld\r\n",iTheta,thetaRHC,ptSource.x,ptSource.y,ptDest.x,ptDest.y);
    }
  </code>

  Remarks:
    GFX_Point provides 32 bit integers for X and Y pixel coordinates.  It is
    assumed that only the lower 16 bits are significant.  All fixed point
    math is calulated in 16 x 16 = 32 bit and rounded to 16 bits.

*/
    GFX_Point RotatePixel( fract_q9d6_t theta,
                           GFX_Point* pCenter,
                           GFX_Point* pSourcePoint );


// *****************************************************************************
/* Function:
    GFX_Point FindSourcePixel( fract_q9d6_t theta,
                               GFX_Point* pCenter,
                               GFX_Point* pDestPoint );

  Summary:
    Returns the source point defined by the destination point, given rotation
    angle and center of rotation.

  Description:
    Rotates source point arount rotation center by angle defined by theta in degress.

  Precondition:
    None

  Parameters:
    theta - Q9.6 fixed point angle
    pCenter - pointer to rotation center
    pDestPoint - pointer to the destination point

  Returns:
    Pixel coordinates the source point that produced the destination point.

  Example:
  <code>
    fract_q10d5_t xSource, ySource;
    fract_q10d5_t xDest, yDest;
    fract_q10d5_t xCenter, yCenter;
    GFX_Point ptSource, ptDest, ptCenter;
    fract_q9d6_t thetaRHC; // Theta in right hand coordinate system;
    fract_q9d6_t thetaLHC; // Theta in left hand coordinate system of display;
    fract_q9d6_t theta0 = 0;
    fract_q9d6_t deltaTheta;
    int iTheta;
    int nTheta = 361;

    deltaTheta = (fract_q9d6_t)( (360.0/(nTheta-1))*(1<<6) );

  //Xcenter = (400,300) on SVGA display
    xCenter = 400<<5; //Scale to Q10.5
    yCenter = 300<<5;
    ptCenter.x = xCenter;
    ptCenter.y = yCenter;

    //Xdest = (600,300) on SVGA Display
    xDest = 600<<5; //Scale to Q10.5
    yDest = 300<<5;
    ptDest.x = xDest;
    ptDest.y = yDest;

    printf("\r\nDest-->Source\r\n");
    for (iTheta = 0; iTheta < nTheta; iTheta++)
    {
        thetaRHC = theta0 + iTheta*deltaTheta;
        thetaLHC = -thetaRHC;
        ptSource = FindSourcePixel(thetaLHC,&ptCenter,&ptDest);
        printf("%d, %d, %ld,%ld, %ld,%ld\r\n",iTheta,thetaRHC,ptDest.x,ptDest.y,ptSource.x,ptSource.y);
    }
  </code>

  Remarks:
    GFX_Point provides 32 bit integers for X and Y pixel coordinates.  It is
    assumed that only the lower 16 bits are significant.  All fixed point
    math is calulated in 16 x 16 = 32 bit and rounded to 16 bits.

*/
    GFX_Point FindSourcePixel( fract_q9d6_t theta,
                               GFX_Point* pCenter,
                               GFX_Point* pDestPoint );

#endif //#ifndef _PIXEL_ROTATION_H_
