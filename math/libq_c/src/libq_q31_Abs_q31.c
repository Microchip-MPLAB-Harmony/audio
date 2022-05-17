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

//******************************************************************************
// File: libq_q31_Abs_q31.c
//
// Description: Creates a saturated Absolute value.
//******************************************************************************

#include "audio/math/libq_c/libq_c.h"


//******************************************************************************
//
// libq_q31_Abs_q31()
//
// Description:
//   Creates a saturated Absolute value.  It takes the absolute value of the
//   32-bit 2s-complement fractional input with saturation. The saturation is
//   for handling the case where taking the absolute value of MINFRACT32 is
//   greater than MAXFRACT32, or the allowable range of 32-bit values.
//   This function relates to the ETSI L_abs function.
//
// Arguments:
//   q31 a [in] input argument
//
// Return Value:
//   q31 result [return]  abs(input) <= MAXFRACT32
//
//*****************************************************************************
q31 libq_q31_Abs_q31(q31 a)
{
    q31 result;  
  
    if (a >= (q31)0)
    {
        // positive 2's complement value 
        result = a;
    }
    else
    {   
        // Negative value, so check for min value (-1) 
        if (a != (q31)MINFRACT32)
        {
            //If not the min value (-1), just negate the input for absolute value 
            result = (q31)(-a);
        }
        //the min value (-1), so saturate to max value (1)
        else
        {
            result = (q31)MAXFRACT32;
        }
    }
    return (result);
}
