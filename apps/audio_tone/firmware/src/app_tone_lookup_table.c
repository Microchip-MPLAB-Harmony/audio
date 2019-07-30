/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_tone_lookup_table.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015-2016 released Microchip Technology Inc.  All rights reserved.

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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <math.h>   // for M_PI

#include "app_tone_lookup_table.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variable Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct 
{
    /* Left channel data */
    int32_t leftData;

    /* Right channel data */
    int32_t rightData;      // removed Pad from end of name to be same as others

} DRV_I2S_DATA32m;

#if AUDIO_FORMAT_WIDTH==16
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Tone_Lookup_Table_tone1[MAX_AUDIO_NUM_SAMPLES];
DRV_I2S_DATA16 __attribute__ ((aligned (32))) App_Tone_Lookup_Table_tone2[MAX_AUDIO_NUM_SAMPLES];
#endif
#if AUDIO_FORMAT_WIDTH==32
DRV_I2S_DATA32m __attribute__ ((aligned (32))) App_Tone_Lookup_Table_tone1[MAX_AUDIO_NUM_SAMPLES];
DRV_I2S_DATA32m __attribute__ ((aligned (32))) App_Tone_Lookup_Table_tone2[MAX_AUDIO_NUM_SAMPLES];
#endif

/* This is initialization function */

void APP_TONE_LOOKUP_TABLE_Initialize(AUDIO_CODEC_DATA* codecData, uint8_t whichBuffer, uint32_t sampleRate, uint16_t *numSamples, uint16_t numNumSamples)
{
    uint16_t i,j,k;
   
#if AUDIO_FORMAT_WIDTH==16
DRV_I2S_DATA16 *lookupTable;
#endif
#if AUDIO_FORMAT_WIDTH==32
DRV_I2S_DATA32m *lookupTable;
#endif

    if (whichBuffer==1)
    {
        codecData->txbufferObject1 = (uint8_t *) App_Tone_Lookup_Table_tone1;
        lookupTable = App_Tone_Lookup_Table_tone1;
    }
    else
    {
        codecData->txbufferObject2 = (uint8_t *) App_Tone_Lookup_Table_tone2;
        lookupTable = App_Tone_Lookup_Table_tone2;              
    }

    // generate sine table
    // radians = degrees × pi / 180°
    k = 0;

    for (j=0; j < numNumSamples; j++)
    {
        uint16_t currNumSamples;
        
        currNumSamples = numSamples[j];
        
        if ((sampleRate%currNumSamples)==0)
        {
            for (i=0; i <= currNumSamples/4; i++)
            {
                double radians = (M_PI*(double)(360.0/(double)currNumSamples)*(double)i)/180.0;
                //double radians = (M_PI*(double)3.0*(double)i)/180.0;

#if AUDIO_FORMAT_WIDTH==32
                lookupTable[i+k].leftData = (int32_t)(0x7FFFFFFF*sin(radians));   // 0 to 0x7FFFFFFF for 0-30 samples
#endif
#if AUDIO_FORMAT_WIDTH==16
                lookupTable[i+k].leftData = (int16_t)(0x7FFF*sin(radians));   // 0 to 0x7FFFFFFF for 0-30 samples        
#endif
                lookupTable[i+k].rightData = lookupTable[i].leftData;        
                if (i<currNumSamples/4)        
                {
                    lookupTable[currNumSamples/2-i+k].leftData = lookupTable[i+k].leftData;   // 0 to 0x7FD317B3 for 60 to 31
                    lookupTable[currNumSamples/2-i+k].rightData = lookupTable[currNumSamples/2-i+k].leftData;        
                }
                if (i)
                {
                    lookupTable[currNumSamples/2+i+k].leftData = -lookupTable[i+k].leftData;   // 0 to 0x80000001 for 61 to 90
                    lookupTable[currNumSamples/2+i+k].rightData = lookupTable[currNumSamples/2+i+k].leftData;        
                    if (i<currNumSamples/4)        
                    {
                        lookupTable[currNumSamples-i+k].leftData = -lookupTable[i+k].leftData;  // 0xF94D0E2E to 0x802CE84D for 119 to 91
                        lookupTable[currNumSamples-i+k].rightData = lookupTable[currNumSamples-i+k].leftData;        
                    }
                }              
            } 
        }
        else
        {
            for (i=0; i < currNumSamples; i++)
            {
                double radians = (M_PI*(double)(360.0/(double)currNumSamples)*(double)i)/180.0;

    #if AUDIO_FORMAT_WIDTH==32
                lookupTable[i+k].leftData = (int32_t)(0x7FFFFFFF*sin(radians));   // 0 to 0x7FFFFFFF for 0-30 samples
    #endif
    #if AUDIO_FORMAT_WIDTH==16
                lookupTable[i+k].leftData = (int16_t)(0x7FFF*sin(radians));   // 0 to 0x7FFFFFFF for 0-30 samples        
    #endif
                lookupTable[i+k].rightData = lookupTable[i+k].leftData;        
            }
        }
        k += currNumSamples;
    }
    
    if (whichBuffer==1)
    {
        codecData->bufferSize1 = sizeof(App_Tone_Lookup_Table_tone1[0])*k;
    }
    else
    {
        codecData->bufferSize2 = sizeof(App_Tone_Lookup_Table_tone2[0])*k;          
    }    
}