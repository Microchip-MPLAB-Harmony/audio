/*******************************************************************************
Microchip DSP Library
    Blackman Windows Initialization

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_transform_window_init_blackman.c

  Summary:
    Create a Blackman window.

  Description:
    Create a N-element Blackman Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Both 16-bit and 32-bit functions are provided.  Output values
    are in Q15 fractional format and Q31 fractional format respectively.
    The Blackman Window follows the equation:

       Window(n) = 0.42659 - 0.49656 * COS(2*Pi*n/(N-1)) + 0.076849 * COS(4*Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples
   
   Notes:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*******************************************************************************/

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
/****************************************************************************
Change log

new content     C15637          2013-10-10

****************************************************************************/

#include <math.h>
#include <stdint.h>
#include "audio/math/audio_math/dsp/dsp.h"


void DSP_TransformWinInit_Black16(int16_t *OutWindow, int N)
{
	int i;
        double Window;
       	
	for (i = 0; i < N; i++)
	{
            Window = 0.42659 - 0.49656  * cos((2*M_PI * i) / (N-1)) + 0.076849 * cos((4*M_PI*i)/(N-1)); // window function
            if (Window >= 1.0)                        // handle saturation
            {
                OutWindow[i] = 0x7FFF;
            }
            else
            {
                OutWindow[i] = Window * pow(2,15);    // convert to Q15
            }
        }
}

void DSP_TransformWinInit_Black32(int32_t *OutWindow, int N)
{
	int i;
        long double Window;
                
	for (i = 0; i < N; i++)
	{
            Window = 0.42659 - 0.49656  * cos((2*M_PI * i) / (N-1)) + 0.076849 * cos((4*M_PI*i)/(N-1)); // window function
            if (Window >= 1.0)                        // handle saturation
            {
                OutWindow[i] = 0x7FFFFFFF;
            }
            else
            {
                OutWindow[i] = Window * pow(2,31);    // convert to Q31
            }
        }
}
