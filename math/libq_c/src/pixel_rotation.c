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

#include "audio/math/libq_c/include/pixel_rotation.h"

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
GFX_Point RotatePixel( fract_q9d6_t theta,
                       GFX_Point* pCenter,
                       GFX_Point* pSourcePoint )
{
    static bool bHaveOldTheta = false;
    bool bHaveNewTheta;
    static fract_q9d6_t thetaOld;
    GFX_Point ptCentered, pDest;
    fract_q0d31_t sinTheta=0, cosTheta=0; //Initialized to remove compiler nags
    
    if (!bHaveOldTheta)
    {//At first use...
        thetaOld = theta;  //Remember angle
        bHaveOldTheta = true;
        bHaveNewTheta = true;
    }
    else if ( thetaOld != theta )
    {//Need to recalculate sin and cosine.
        bHaveNewTheta = true;
        thetaOld = theta;
    }
    else
    {
        bHaveNewTheta = false;
    }
    
    if ( bHaveNewTheta )
    {//Recalculate sin and cosine.
     //Promote results to 32 bits to do 16x2=32 bit multiplies below.
        sinTheta = (fract_q0d31_t)SinInDegs(theta); 
        cosTheta = (fract_q0d31_t)CosInDegs(theta);
    }
    
  //Calculate point to be rotated.
    ptCentered.x = pSourcePoint->x - pCenter->x;
    ptCentered.y = pSourcePoint->y - pCenter->y;
    
  //Apply transform using full precision multiplies
    pDest.x = cosTheta*ptCentered.x - sinTheta*ptCentered.y + ((pCenter->x)<<15);
    pDest.y = sinTheta*ptCentered.x + cosTheta*ptCentered.y + ((pCenter->y)<<15);
    
  //Round to 16 significant bits
    pDest.x = (pDest.x + (0x1<<14))>>15;
    pDest.y = (pDest.y + (0x1<<14))>>15;
    
    return pDest;

}//RotatePixel


#define Q0D15_ONE (0x00007FFF) //"One" in Q0.15 fixed point.  Used below.
GFX_Point FindSourcePixel( fract_q9d6_t theta,
                           GFX_Point* pCenter,
                           GFX_Point* pDestPoint )
{
    static bool bHaveOldTheta = false;
    bool bHaveNewTheta;
    static fract_q9d6_t thetaOld;
    GFX_Point ptOne, ptTwo, ptSource;
    fract_q0d31_t sinTheta=0, cosTheta=0; //Initialized to remove compiler nags
    
    if (!bHaveOldTheta)
    {//At first use...
        thetaOld = theta;  //Remember angle
        bHaveOldTheta = true;
        bHaveNewTheta = true;
    }
    else if ( thetaOld != theta )
    {//Need to recalculate sin and cosine.
        bHaveNewTheta = true;
        thetaOld = theta;
    }
    else
    {
        bHaveNewTheta = false;
    }
    
    if ( bHaveNewTheta )
    {//Recalculate sin and cosine.
     //Promote results to 32 bits to do 16x6=32 bit multiplies below.
        sinTheta = (fract_q0d31_t)SinInDegs(theta); 
        cosTheta = (fract_q0d31_t)CosInDegs(theta);
    }

  //Apply transform using full precision multiplies    
    ptOne.x =  cosTheta*pDestPoint->x + sinTheta*pDestPoint->y;
    ptOne.y = -sinTheta*pDestPoint->x + cosTheta*pDestPoint->y;
    
    ptTwo.x = (Q0D15_ONE - cosTheta)*pCenter->x - sinTheta*pCenter->y;
    ptTwo.y = sinTheta*pCenter->x + (Q0D15_ONE - cosTheta)*pCenter->y;
    
    ptSource.x = ptOne.x + ptTwo.x;
    ptSource.y = ptOne.y + ptTwo.y;
    
  //Round to 16 significant bits
    ptSource.x = (ptSource.x + (0x1<<14))>>15;
    ptSource.y = (ptSource.y + (0x1<<14))>>15;    
    
    return ptSource;
    
}//FindSourcePixel
#pragma GCC diagnostic pop 
