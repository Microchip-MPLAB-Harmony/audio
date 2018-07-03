/*******************************************************************************
 
  Audio Codec API

  Company:
    Microchip Technology Inc.

  File Name:
    Audio_Codec.c

  Summary:
   Contains the functional implementation of CODEC object instantiated 
   in APP.

  Description:

   Abstract object--

   typedef struct
   {
       Audio_Codec_STATES state;
       Audio_Codec_CLIENT codecClient;
   } AUDIO_CODEC_DATA;

   functions

     void Audio_Codec_Initialize (AUDIO_CODEC_DATA *);
     void Audio_Codec_BufferEventHandler( DRV_CODEC_BUFFER_EVENT event,
                                        DRV_CODEC_BUFFER_HANDLE handle, 
                                        uintptr_t context );
     void Audio_Codec_CommandEventHandler(uintptr_t context);
     void APP_CODEC_CommandClear();

     bool Audio_Codec_Addbuffer(AUDIO_CODEC_DATA *, int8_t *, size_t);
     bool Audio_Codec_Open(AUDIO_CODEC_DATA *);
     void Audio_Codec_SetBufferHandler(AUDIO_CODEC_DATA *);
     void Audio_Codec_SetCommandCallback(AUDIO_CODEC_DATA *);
     void Audio_Codec_Close(AUDIO_CODEC_DATA *);
     bool Audio_Codec_VolumeSet(AUDIO_CODEC_DATA * codecData, uint8_t mVolume);
     uint32_t Audio_Codec_SamplingRateGet(AUDIO_CODEC_DATA * codecData);

   Requires-- 
     APP interface function to receive buffer complete signal - 
         void Audio_Codec_TxBufferComplete(); 

   TODO: CommandEventHandler and BufferEventHandler should be in APP, as the
         object is instantiated from the APP.  Rather than have the interface
         function to receive the buffer complete signal.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"    // also brings in Audio_Codec.h

//******************************************************************************
//
// Audio_Codec_Initialize()
//
// Description:
//    Called from the APP_Initialize() function to initialize the codecData
//    struct.
//
// Arguments:
//    [in] appData has member codecData initialized here to set the
//         buffer and command event handlers.
//
//******************************************************************************
void Audio_Codec_Initialize (AUDIO_CODEC_DATA* codecData)
{
    codecData->codecClient.handle = DRV_HANDLE_INVALID;
    codecData->codecClient.context = 0;
    codecData->codecClient.bufferHandler = 
           (DRV_CODEC_BUFFER_EVENT_HANDLER) Audio_Codec_BufferEventHandler;
    codecData->codecClient.txbufferObject1 = NULL;
    //codecData->codecClient.txbufferObject2 = NULL;
    codecData->codecClient.bufferSize1 = 0;
    //codecData->codecClient.bufferSize2 = 0;
    codecData->codecClient.commandHandler = 
           (DRV_CODEC_COMMAND_EVENT_HANDLER) Audio_Codec_CommandEventHandler;   // future
    //codecData->codecClient.currentCommand = CODEC_COMMAND_NONE;   // future
}

//******************************************************************************
//
// Audio_Codec_SetBufferEventHandler()
//
// Description
//    Set the buffer handler to the current codecData instance.
//
// Argument
//  [out] codecData recieves the codec driver instance handle. 
//
//******************************************************************************
void Audio_Codec_SetBufferEventHandler(AUDIO_CODEC_DATA* codecData)
{
    DRV_CODEC_BufferEventHandlerSet(codecData->codecClient.handle, 
                                    codecData->codecClient.bufferHandler, 
                                    codecData->codecClient.context);
}

//******************************************************************************
//
// Audio_Codec_SetCommandCallback()
//
// Description
//    Sets the command event complete callback routine to the current
//    codecData instance. 
//
// Argument
//  [in] codecData has the client setup data from the APP initialization
//
//******************************************************************************
void Audio_Codec_SetCommandCallback(AUDIO_CODEC_DATA* codecData)
{
    DRV_CODEC_CommandEventHandlerSet(codecData->codecClient.handle, 
                                     codecData->codecClient.commandHandler, 
                                     codecData->codecClient.context);
}

void Audio_Codec_CommandEventHandler( uintptr_t context )
{
    // command executed successfully
}

//******************************************************************************
//
// Audio_Codec_Open()
//
// Description
//    Open the codec.
//
// Argument
//  [out] codecData receives the codec driver instance handle. 
//
// Returns: true if handle is valid
//
//******************************************************************************
bool Audio_Codec_Open(AUDIO_CODEC_DATA* codecData)
{
    SYS_STATUS status;

    status = DRV_CODEC_Status(sysObjdrvCodec0);     // see if codec is done initializing
    if (SYS_STATUS_READY == status)
    {
        // This means the driver can now be be opened.
        /* A client opens the driver object to get an Handle */
        codecData->codecClient.handle = DRV_CODEC_Open(DRV_CODEC_INDEX_0, 
                                                       DRV_IO_INTENT_WRITE | 
                                                       DRV_IO_INTENT_EXCLUSIVE);       
        if(codecData->codecClient.handle != DRV_HANDLE_INVALID)
        {
            return true;
        }
   }
   return false;
}


//******************************************************************************
//
// Audio_Codec_Close()
//
// Description
//    Close the codec.
//
// Argument
//  [in] codecData has the codec driver instance handle. 
//
// Returns: true if handle is valid
//
//******************************************************************************
void Audio_Codec_Close(AUDIO_CODEC_DATA* codecData)
{
    DRV_CODEC_Close(codecData->codecClient.handle);
}

//******************************************************************************
//
// Audio_Codec_Addbuffer()
//
// Description
//  Add a buffer to the write queue
//
// Argument
//  [out] codecData receives the codec driver instance handle. 
//
// Returns: true if write buffer handle is valid
//
//******************************************************************************
bool Audio_Codec_Addbuffer(AUDIO_CODEC_DATA * codecData, DRV_CODEC_BUFFER_HANDLE*
                           writeBufHandle, uint8_t* buffer, size_t bufferSize)
{
    DRV_CODEC_BufferAddWrite(codecData->codecClient.handle, writeBufHandle,
                             buffer, bufferSize);
            
    if(*writeBufHandle != DRV_CODEC_BUFFER_HANDLE_INVALID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//******************************************************************************
//
// Audio_Codec_BufferEventHandler()
//
// Description
//   Application CODEC buffer Event handler called back by the CODEC driver 
//   when a CODEC data buffer TX completes. Indicates when head buffer
//   has been completed transmission.
//
//   NOTE: Required callback routine by CODEC driver.
//
// Arguments: 
//     event   - Action completed
//     handle  - driver instance handle
//     context - operation context
//
//******************************************************************************
void Audio_Codec_BufferEventHandler(DRV_CODEC_BUFFER_EVENT event,
                                  DRV_CODEC_BUFFER_HANDLE handle, 
                                  uintptr_t context)
{
    switch(event)
    {
        case DRV_CODEC_BUFFER_EVENT_COMPLETE:
        {
            //Signal to APP that Tx is complete. 
            Audio_Codec_TxBufferComplete();         // must be defined in app.c
        }        
        break;

        case DRV_CODEC_BUFFER_EVENT_ERROR:
        {
        } 
        break;

        case DRV_CODEC_BUFFER_EVENT_ABORT:
        {
        } 
        break;
    }
}

void Audio_Codec_SamplingRateSet(AUDIO_CODEC_DATA* codecData, uint32_t samplingRate)
{
    DRV_CODEC_SamplingRateSet(codecData->codecClient.handle, samplingRate);
}

uint32_t Audio_Codec_SamplingRateGet(AUDIO_CODEC_DATA* codecData)
{
    return DRV_CODEC_SamplingRateGet(codecData->codecClient.handle);
}

void Audio_Codec_MuteOn(AUDIO_CODEC_DATA* codecData)
{
    return DRV_CODEC_MuteOn(codecData->codecClient.handle);
}

void Audio_Codec_MuteOff(AUDIO_CODEC_DATA* codecData)
{
    return DRV_CODEC_MuteOff(codecData->codecClient.handle);
}

bool Audio_Codec_VolumeSet(AUDIO_CODEC_DATA * codecData, uint8_t mVolume)
{
    DRV_CODEC_VolumeSet(codecData->codecClient.handle, DRV_CODEC_CHANNEL_LEFT_RIGHT, mVolume);
    return true;        // return value not used, it exists to be compatible with other codec API's
}

uint8_t Audio_Codec_VolumeGet(AUDIO_CODEC_DATA * codecData)
{
    return DRV_CODEC_VolumeGet(codecData->codecClient.handle, DRV_CODEC_CHANNEL_LEFT_RIGHT);
}