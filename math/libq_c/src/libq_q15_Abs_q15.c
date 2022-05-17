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

//*****************************************************************************
// File: libq_q15_Abs_q15.c
//
// Description: Creates a saturated Absolute value.
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

/******************************************************************************
 *
 * libq_q15_Abs_q15()
 *
 * Description:
 *   Saturated Absolute value.  It takes the absolute value of the
 *   16-bit 2s-complement fractional input with saturation. The saturation is
 *   for handling the case where taking the absolute value of MINFRACT16 is
 *   greater than MAXFRACT16, or the allowable range of 16-bit values.
 *   This function relates to the ETSI abs function.
 *
 * Arguments:
 *   q15 a [in] input argument
 *
 * Return Value:
 *   q15 result [return]  abs(input) <= MAXFRACT16
 ******************************************************************************/
q15 libq_q15_Abs_q15(q15 a)
{
    q15 result;                   /* Value returned */
  
    if (a >= (q15)0)
    {
        result = (q15)(a);
    }
    else
    {
        //Negative value, 
        if (a != (q15)MINFRACT16)
        {
            //Not min value (-1) 
            result = (q15)(-a);
        }
        // The min value (-1), so saturate to max value (1) */
        else
        {
            result = (q15)MAXFRACT16;
        }
    }
    return (result);
}
