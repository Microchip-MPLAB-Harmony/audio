/*******************************************************************************
 
  Audio Codec Interface

  Company:
    Microchip Technology Inc.

  File Name:
    Audio_Codec.h

  Summary:
    Audio Codec Interface 

  Description:
    This file describes the audio codec interface routines. 
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

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
#ifndef Audio_Codec_H
#define Audio_Codec_H

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* CODEC states

  Summary:
    CODEC states enumeration

  Description:
    This enumeration defines the valid codec states.  These states
    determine the behavior of the codec at various times.
*/

typedef struct
{
    DRV_HANDLE handle;
    DRV_CODEC_BUFFER_HANDLE writeBufHandle1, writeBufHandle2;
    DRV_CODEC_BUFFER_EVENT_HANDLER bufferHandler;
    DRV_CODEC_COMMAND_EVENT_HANDLER commandHandler;
    uintptr_t context;
    uint8_t *txbufferObject1;
    uint8_t *txbufferObject2;
    size_t bufferSize1, bufferSize2;
} Audio_Codec_CLIENT;


// *****************************************************************************
// CODEC Data
//
//  Summary:
//    Holds codec data
//
//  Description:
//    This structure holds the codec's data.
//
//  Remarks:
// *****************************************************************************
typedef struct
{
    /* Application's current state*/
    Audio_Codec_CLIENT codecClient;

} AUDIO_CODEC_DATA;


/*******************************************************************************
  Function:
    void Audio_Codec_Initialize ( void )

  Summary:
     Codec initialization routine

  Description:
    This routine initializes the Codec.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "APP_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    Audio_Codec_Initialize();
    </code>

  Remarks:
    This routine must be called from the APP_Initialize function.
*/
 void Audio_Codec_Initialize (AUDIO_CODEC_DATA *);


/*******************************************************************************
  Function:
    void Audio_Codec_BufferEventHandler( DRV_CODEC_BUFFER_EVENT event,
                                       DRV_CODEC_BUFFER_HANDLE handle, 
                                       uintptr_t context )

  Description:
    This is the Event Handler for Codec Tx and Rx Complete Events.

  Parameters:
    None.

  Returns: None.

*/
void Audio_Codec_BufferEventHandler( DRV_CODEC_BUFFER_EVENT event,
                                   DRV_CODEC_BUFFER_HANDLE handle, 
                                   uintptr_t context );

void Audio_Codec_SetBufferEventHandler(AUDIO_CODEC_DATA * codecData);
void Audio_Codec_CommandEventHandler(uintptr_t context);
bool Audio_Codec_Addbuffer(AUDIO_CODEC_DATA *, DRV_CODEC_BUFFER_HANDLE *, uint8_t *, size_t);
bool Audio_Codec_Open(AUDIO_CODEC_DATA *);
void Audio_Codec_Close(AUDIO_CODEC_DATA *);
void Audio_Codec_SetCommandCallback(AUDIO_CODEC_DATA *);
bool Audio_Codec_VolumeSet(AUDIO_CODEC_DATA * codecData, uint8_t mVolume);
uint8_t Audio_Codec_VolumeGet(AUDIO_CODEC_DATA * codecData);
void Audio_Codec_SamplingRateSet(AUDIO_CODEC_DATA * codecData, uint32_t samplingRate);
uint32_t Audio_Codec_SamplingRateGet(AUDIO_CODEC_DATA * codecData);
void Audio_Codec_MuteOn(AUDIO_CODEC_DATA *);
void Audio_Codec_MuteOff(AUDIO_CODEC_DATA *);

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

#endif

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END
